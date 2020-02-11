/*--------------------------------------------------------------------
  (C) Copyright 2006-2015 Barcelona Supercomputing Center
                          Centro Nacional de Supercomputacion
  
  This file is part of Mercurium C/C++ source-to-source compiler.
  
  See AUTHORS file in the top level directory for information
  regarding developers and contributors.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  Mercurium C/C++ source-to-source compiler is distributed in the hope
  that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the GNU Lesser General Public License for more
  details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with Mercurium C/C++ source-to-source compiler; if
  not, write to the Free Software Foundation, Inc., 675 Mass Ave,
  Cambridge, MA 02139, USA.
--------------------------------------------------------------------*/

#include"tl-omp-lowering-atomics.hpp"
#include"tl-nodecl-utils.hpp"
#include"tl-counters.hpp"

namespace TL { namespace OpenMP { namespace Lowering {

    namespace
    {
        // FIXME - Part of this logic must be moved to OpenMP::Core
        bool atomic_binary_check_expr(Nodecl::NodeclBase lhs_assig, Nodecl::NodeclBase lhs, Nodecl::NodeclBase rhs)
        {
            // Purely syntactic check
            return (Nodecl::Utils::structurally_equal_nodecls(lhs_assig, lhs, /* skip_conversion_nodecls */ true)
                    != Nodecl::Utils::structurally_equal_nodecls(lhs_assig, rhs, /* skip_conversion_nodecls */ true));
        }

        bool is_valid_type_fortran(TL::Type t)
        {
            return (!t.no_ref().is_fortran_array()
                    && !(t.no_ref().is_pointer()
                        && t.no_ref().points_to().is_fortran_array()));
        }


        bool allowed_expression_atomic_fortran(Nodecl::NodeclBase expr, bool &using_builtin, bool &using_nanos_api)
        {
            if (!expr.is<Nodecl::Assignment>())
            {
                // std::cerr << "NOT AN ASSIGNMENT!" << std::endl;
                return false;
            }

            Nodecl::NodeclBase lhs_assig = expr.as<Nodecl::Assignment>().get_lhs();
            Nodecl::NodeclBase rhs_assig = expr.as<Nodecl::Assignment>().get_rhs();

            if (!is_valid_type_fortran(lhs_assig.get_type())
                    || !is_valid_type_fortran(rhs_assig.get_type()))
                return false;

            node_t op_kind = rhs_assig.get_kind();

            switch (op_kind)
            {
                case NODECL_ADD:
                case NODECL_MUL:
                case NODECL_MINUS:
                case NODECL_DIV:
                case NODECL_LOGICAL_AND:
                case NODECL_LOGICAL_OR:
                    {
                        Nodecl::NodeclBase lhs = rhs_assig.as<Nodecl::Add>().get_lhs();
                        Nodecl::NodeclBase rhs = rhs_assig.as<Nodecl::Add>().get_rhs();
                        if (!atomic_binary_check_expr(lhs_assig,
                                    lhs,
                                    rhs))
                            return false;
                    }
                    break;
                case NODECL_EQUAL:
                case NODECL_DIFFERENT:
                    {
                        Nodecl::NodeclBase lhs = rhs_assig.as<Nodecl::Add>().get_lhs();
                        Nodecl::NodeclBase rhs = rhs_assig.as<Nodecl::Add>().get_rhs();
                        if (!atomic_binary_check_expr(lhs_assig,
                                    lhs,
                                    rhs))
                            return false;

                        // Only .EQV. and .NEQV.
                        if (!lhs.get_type().no_ref().is_bool()
                                || !rhs.get_type().no_ref().is_bool())
                            return false;
                    }
                    break;
                case NODECL_FUNCTION_CALL:
                    {
                        Nodecl::NodeclBase called = rhs_assig.as<Nodecl::FunctionCall>().get_called();
                        TL::Symbol called_sym = called.get_symbol();
                        if (!called_sym.is_valid())
                        {
                            // std::cerr << "SYM NOT VALID" << std::endl;
                            return false;
                        }
                        if (!called_sym.is_builtin())
                        {
                            // std::cerr << "SYM NOT BUILTIN" << std::endl;
                            return false;
                        }
                        if (called_sym.get_name() != "max"
                                && called_sym.get_name() != "min"
                                && called_sym.get_name() != "ieor"
                                && called_sym.get_name() != "ior"
                                && called_sym.get_name() != "iand")
                        {
                            // std::cerr << "NOT VALID BUILTIN" << std::endl;
                            return false;
                        }

                        Nodecl::List args = rhs_assig.as<Nodecl::FunctionCall>().get_arguments().as<Nodecl::List>();
                        if (args.size() != 2)
                        {
                            // std::cerr << "ARGS NOT 2" << std::endl;
                            return false;
                        }

                        Nodecl::NodeclBase arg_0 = args[0];
                        Nodecl::NodeclBase arg_1 = args[1];

                        if (arg_0.is<Nodecl::FortranActualArgument>())
                            arg_0 = arg_0.as<Nodecl::FortranActualArgument>().get_argument();
                        if (arg_1.is<Nodecl::FortranActualArgument>())
                            arg_1 = arg_1.as<Nodecl::FortranActualArgument>().get_argument();

                        if (!atomic_binary_check_expr(lhs_assig,
                                    arg_0, arg_1))
                        {
                            // std::cerr << "NOT OF THE FORM X=F(X,Y) or X=F(Y,X)" << std::endl;
                            // std::cerr << " LHS_ASSIG -> " << lhs_assig.prettyprint() << std::endl;
                            // std::cerr << " ARGS0 -> " << arg_0.prettyprint() << std::endl;
                            // std::cerr << " ARGS1 -> " << arg_1.prettyprint() << std::endl;
                            return false;
                        }

                        if (!is_valid_type_fortran(arg_0.get_type())
                                || !is_valid_type_fortran(arg_1.get_type()))
                            return false;
                    }
                    break;
                default:
                    // std::cerr << "??? " << ast_print_node_type(op_kind) << std::endl;
                    return false;
            }

            using_nanos_api = true;
            return true;
        }


        bool allowed_expression_atomic_c(Nodecl::NodeclBase expr, bool &using_builtin, bool &using_nanos_api)
        {
            node_t op_kind = expr.get_kind();

            switch (op_kind)
            {
                case NODECL_PREINCREMENT :
                case NODECL_POSTINCREMENT :
                case NODECL_PREDECREMENT :
                case NODECL_POSTDECREMENT:
                    {
                        // They have the same tree
                        Nodecl::NodeclBase operand = expr.as<Nodecl::Preincrement>().get_rhs();

                        Type t = operand.get_type();

                        if (t.is_any_reference())
                            t = t.references_to();

                        if (!(t.is_integral_type()
                                    || t.is_floating_type()))
                            return false;

                        // Only integer types can use a gcc builtin
                        using_builtin = t.is_integral_type();

                        // No objections
                        return true;
                    }
                case NODECL_ADD_ASSIGNMENT:
                case NODECL_MINUS_ASSIGNMENT:
                case NODECL_MUL_ASSIGNMENT:
                case NODECL_DIV_ASSIGNMENT:
                case NODECL_BITWISE_AND_ASSIGNMENT:
                case NODECL_BITWISE_OR_ASSIGNMENT:
                case NODECL_BITWISE_XOR_ASSIGNMENT:
                case NODECL_BITWISE_SHL_ASSIGNMENT:
                case NODECL_ARITHMETIC_SHR_ASSIGNMENT:
                case NODECL_BITWISE_SHR_ASSIGNMENT:
                    {
                        Nodecl::NodeclBase lhs = expr.as<Nodecl::AddAssignment>().get_lhs();

                        Type t = lhs.get_type();

                        bool is_lvalue = t.is_lvalue_reference();

                        if (t.is_any_reference())
                            t = t.references_to();

                        bool lhs_is_integral = t.is_integral_type();
                        if (!is_lvalue
                                || !(lhs_is_integral
                                    || t.is_floating_type()))
                            return false;

                        // Likewise for rhs
                        Nodecl::NodeclBase rhs = expr.as<Nodecl::AddAssignment>().get_rhs();

                        t = rhs.get_type();

                        if (t.is_any_reference())
                            t = t.references_to();

                        if (!(t.is_integral_type()
                                    || t.is_floating_type()))
                            return false;

                        using_builtin =
                            lhs_is_integral
                            && (op_kind == NODECL_ADD_ASSIGNMENT                 // x += y
                                    || op_kind == NODECL_MINUS_ASSIGNMENT        // x -= y
                                    || op_kind == NODECL_BITWISE_AND_ASSIGNMENT  // x &= y
                                    || op_kind == NODECL_BITWISE_OR_ASSIGNMENT   // x |= y
                                    || op_kind == NODECL_BITWISE_XOR_ASSIGNMENT  // x ^= y
                               );

                        return true;
                    }
                default:
                    return false;
            }

            return false;
        }
    }

    bool allowed_expression_atomic(Nodecl::NodeclBase expr, bool &using_builtin, bool &using_nanos_api)
    {
        if (IS_FORTRAN_LANGUAGE)
            return allowed_expression_atomic_fortran(expr, using_builtin, using_nanos_api);
        else
            return allowed_expression_atomic_c(expr, using_builtin, using_nanos_api);
    }

    Nodecl::NodeclBase compare_and_exchange(Nodecl::NodeclBase expr)
    {
        node_t op_kind = expr.get_kind();
        Source critical_source;
        Source type, lhs, rhs, op, bytes, proper_int_type, temporary;

        Type expr_type = expr.get_type();

        if (expr_type.is_any_reference())
        {
            expr_type = expr_type.references_to();
        }

        Source atomic_union; // Defined below

        critical_source
            << "{"
            <<   type << " __oldval;"
            <<   type << " __newval;"

            <<   temporary


            <<   "do {"
            <<      "__oldval = (" << lhs << ");"
            <<      "__newval = __oldval " << op << " (" << rhs << ");"
            <<      "__sync_synchronize();"
            <<   "} while (!__sync_bool_compare_and_swap_" << bytes << "("
            <<                 "(" << proper_int_type << "*)&(" << lhs << "),"
            <<                 "(" << atomic_union << "){__oldval}.__addr,"
            <<                 "(" << atomic_union << "){__newval}.__addr));"
            << "}"
            ;

        if (op_kind == NODECL_PREINCREMENT  // ++x
                || op_kind == NODECL_POSTINCREMENT // x++
                || op_kind == NODECL_PREDECREMENT // --x
                || op_kind == NODECL_POSTDECREMENT) // x--
        {
            lhs << as_expression(expr.as<Nodecl::Preincrement>().get_rhs());
            rhs << "1";

            if (op_kind == NODECL_PREDECREMENT
                    || op_kind == NODECL_POSTDECREMENT)
            {
                op << "-";
            }
            else
            {
                op << "+";
            }

        }
        else
        {
            lhs << as_expression(expr.as<Nodecl::AddAssignment>().get_lhs());
            op << Nodecl::Utils::get_elemental_operator_of_binary_expression(expr);

            temporary
                << type << " __temp = " << as_expression(expr.as<Nodecl::AddAssignment>().get_rhs()) << ";";
            rhs << "__temp";
        }

        type << as_type(expr_type);
        bytes << expr_type.get_size();

        if (expr_type.get_size() == 4)
        {
            Type int_type(::get_unsigned_int_type());
            if (int_type.get_size() == 4)
            {
                proper_int_type << "int";
            }
            else
            {
                internal_error("Code unreachable", 0);
            }
        }
        else if (expr_type.get_size() == 8)
        {
            // FIXME - Why are we using a low level op?
            Type long_type(::get_unsigned_long_int_type());
            Type long_long_type(::get_unsigned_long_long_int_type());

            if (long_type.get_size() == 8)
            {
                proper_int_type << "long";
            }
            else if (long_long_type.get_size() == 8)
            {
                proper_int_type << "long long";
            }
            else
            {
                internal_error("Code unreachable", 0);
            }
        }

        // Atomic union
        Counter& union_counter = CounterManager::get_counter("nanos-union-atomic");
        atomic_union << "__nanos_atomic_union_" << (int)union_counter;
        union_counter++;
        Source atomic_type_decl;
        atomic_type_decl << "typedef union { " << type << " __val;" << proper_int_type << " __addr; } " << atomic_union << ";"
            ;

        Nodecl::NodeclBase n = atomic_type_decl.parse_global(expr);
        if (!n.is_null())
        {
            Nodecl::Utils::prepend_to_enclosing_top_level_location(expr, n);
        }

        return critical_source.parse_statement(expr);
    }

    Nodecl::NodeclBase builtin_atomic_int_op(Nodecl::NodeclBase expr)
    {
        node_t op_kind = expr.get_kind();
        Source critical_source;
        if (op_kind == NODECL_PREINCREMENT  // ++x
                || op_kind == NODECL_POSTINCREMENT // x++
                || op_kind == NODECL_PREDECREMENT // --x
                || op_kind == NODECL_POSTDECREMENT) // x--
        {
            // FIXME: __sync_add_and_fetch or __sync_fetch_and_add? I think
            // the former would be better for a dumb compiler
            std::string intrinsic_function_name;

            switch (op_kind)
            {
                case NODECL_PREINCREMENT:
                case NODECL_POSTINCREMENT:
                    {
                        intrinsic_function_name = "__sync_add_and_fetch";
                        break;
                    }
                case NODECL_PREDECREMENT:
                case NODECL_POSTDECREMENT:
                    {
                        intrinsic_function_name = "__sync_sub_and_fetch";
                        break;
                    }
                default:
                    internal_error("Code unreachable", 0);
            }

            Source op_size;
            op_size << expr.as<Nodecl::Preincrement>().get_rhs().get_type().no_ref().get_size();

            critical_source << intrinsic_function_name << "_" << op_size << "(&(" << as_expression(expr.as<Nodecl::Preincrement>().get_rhs()) << "), 1);"
                ;
        }
        // No need to check the other case as allowed_expression_atomic
        // already did this for us
        else
        {
            std::string intrinsic_function_name;
            switch ((int)op_kind)
            {
                case NODECL_ADD_ASSIGNMENT : // x += y
                    {
                        intrinsic_function_name = "__sync_add_and_fetch";
                        break;
                    }
                case NODECL_MINUS_ASSIGNMENT : // x -= y
                    {
                        intrinsic_function_name = "__sync_sub_and_fetch";
                        break;
                    }
                    // case NODECL_MUL_ASSIGNMENT:  // x *= y
                    //     {
                    //         // This one does not seem to exist
                    //         intrinsic_function_name = "__sync_mul_and_fetch";
                    //         break;
                    //     }
                    // case NODECL_DIV_ASSIGNMENT:  // x *= y
                    //     {
                    //         // This one does not seem to exist
                    //         intrinsic_function_name = "__sync_div_and_fetch";
                    //         break;
                    //     }
                case NODECL_BITWISE_AND_ASSIGNMENT : // x &= y
                    {
                        intrinsic_function_name = "__sync_and_and_fetch";
                        break;
                    }
                case NODECL_BITWISE_OR_ASSIGNMENT : // x |= y
                    {
                        intrinsic_function_name = "__sync_or_and_fetch";
                        break;
                    }
                case NODECL_BITWISE_XOR_ASSIGNMENT : // x ^= y
                    {
                        intrinsic_function_name = "__sync_xor_and_fetch";
                        break;
                    }
                    // case NODECL_SHL_ASSIGNMENT : // x <<= y
                    //     {
                    //         // This one does not seem to exist
                    //         intrinsic_function_name = "__sync_shl_and_fetch";
                    //         break;
                    //     }
                    // case NODECL_SHR_ASSIGNMENT : // x >>= y
                    //     {
                    //         // This one does not seem to exist
                    //         intrinsic_function_name = "__sync_shr_and_fetch";
                    //         break;
                    //     }
                default:
                    internal_error("Code unreachable", 0);
            }

            Source op_size;
            op_size << expr.as<Nodecl::AddAssignment>().get_rhs().get_type().no_ref().get_size();

            critical_source
                << "{"
                << as_type(expr.as<Nodecl::AddAssignment>().get_rhs().get_type()) << "__tmp = "
                << as_expression(expr.as<Nodecl::AddAssignment>().get_rhs()) << ";"
                << intrinsic_function_name << "_" << op_size << "(&(" << as_expression(expr.as<Nodecl::AddAssignment>().get_lhs()) << "), __tmp);"
                << "}"
                ;
        }

        return critical_source.parse_statement(expr);
    }
}}}
