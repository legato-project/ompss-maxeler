#!/bin/bash -e

#delete old build.xml file
rm -f build.xml
#build build.xml file for application
cat > build.xml << EOF
<?xml version="1.0" encoding="UTF-8"?>

<project name ="gemm" default="all" basedir=".">
	<taskdef name="maxjcompiler" classname="org.eclipse.jdt.core.ant.taskdef.MaxjTask" classpath="$MAXCOMPILERDIR/lib/ecj.jar"/>

	<target name="all" depends="build"/>
	<target name="build">
		<delete dir="bin"/>
		<mkdir dir="bin"/>
		<copy includeemptydirs="false" todir="bin">
			<fileset dir="src" excludes="**/*.uad, **/*.ucd, **/*.uld, **/*.upd, **/*.udd, **/*.uod, **/*.usd, **/*.utd, **/*.uud, **/*.odd, **/*.ead, **/*.ecd, **/*.eld, **/*.epd, **/*.edd, **/*.eod, **/*.esd, **/*.etd, **/*.eud, **/*.urd*.uml, **/*.ecore, **/*.launch, **/*.java, **/*.maxj, **/package.html"/>
		</copy>
		<maxjcompiler srcdir="src" destdir="bin" debug="true" failonerror="true" debuglevel="lines,vars,source" source="1.8" target="1.8">
			<classpath>
				<pathelement path="$MAXCOMPILERDIR/lib/MaxCompiler.jar"/>
				<pathelement path="$MAXCOMPILERDIR/lib/Max5Platform.jar"/>
				<pathelement path="$MAXPOWERDIR/lib/MaxPower.jar"/>
			</classpath>
		</maxjcompiler>
	</target>
	<target name="clean">
		<delete dir="bin"/>
	</target>
</project>
EOF

#build
ant

export APPDIR=.
export MAXAPPPKG=gemm
export MAXJVMMEM=12288
export JUNIT=$MAXCOMPILERDIR/lib/MaxIDE/plugins/org.junit_4.8.1.v4_8_1_v20100427-1100/junit.jar
export HAMCREST=$MAXCOMPILERDIR/lib/MaxIDE/plugins/org.hamcrest.core_1.1.0.v20090501071000.jar
export MAXCOMPILER=$MAXCOMPILERDIR/lib/MaxCompiler.jar:$MAXCOMPILERDIR/lib/Max5Platform.jar
export MAXCOMPILERJCP=$MAXCOMPILER:$JUNIT:$HAMCREST:/network-raid/opt/maxq/maxq-ctl.jar
export MAXAPPJCP=$APPDIR/bin:$MAXPOWERDIR/lib/MaxPower.jar
export MAXSOURCEDIRS=$APPDIR/src
export MAXMPPRMAILTO=$USER@maxeler.com

#run
exec 5>&1
buildOutput=$(maxJavaRun GemmManagerMax5 DFEModel=MAIA maxFileName=gemm target=$1 "${@:2}" | tee >(cat - >&5))
result=$?

# Regex to select "MaxFile: /path/to/build/dir/MaxFileName" from the output of maxJavaRun
if [[ "${buildOutput}" =~ MaxFile:[[:space:]][^.]+ ]]; then
    includingMaxFile=${BASH_REMATCH}
    # Get rid of the front "MaxFile: " so we just have the file location without extension
    if [[ $includingMaxFile =~ /[^.]+  ]]; then
        cp ${BASH_REMATCH}.max cpu_src/
        cp ${BASH_REMATCH}.h cpu_src/
	exit $result
    fi
fi
echo Could not find MaxFile
exit -1
