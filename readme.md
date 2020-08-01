# Introduction

This project is a tool for creating a Windows executable that launches a Java application which is bundled with a JRE.

The generated exe-file is around 200 kb if compiled with MSVC, and will normally not need to be updated unless you rename your main Java class or change the application icon. That means that the most convenient probably is to add the compiled exe-file to your project's Git repository and just include in the package when generating a release-bundle, but you can of course also add the compilation step as a part of the build process.

The file structure of the deployed Java application would typically look something like this

	my_project/
		myapp.exe        # the launcher created by this project
		jre/             # the bundled JRE
		lib/
			my_app.jar    # your Java application
			...           # any third-party libraries, etc.

When the user launches myapp.exe, it will launch your Java application with the bundled JRE, with a command that looks like ```jre\bin\java.exe --class-path "lib/*" my.package.MyApplication```

# Usage instructions

Edit the first line in **launcher.cpp**:

	#define CLASSNAME      L"my.package.MyApplication"

Replace ```my.package.MyApplication``` with the name of your main Java class (the class that has the ```main``` method)

You can replace **icon.ico** with your own icon, or if you don't want to use an icon, just remove the things related to resources.res from the commands below.

## Compile the executable:

### On Windows, using MSVC command line tools

	CL /O1 /EHsc -c launcher.cpp
	RC /fo resources.res resources.rc
	LINK /OUT:launcher.exe launcher.obj resources.res user32.lib shlwapi.lib

### On Linux, using Mingw-w64 toolchain

It is possible to cross-compile the Windows executable from Linux, using the x86_64-w64-mingw32 toolchain, although this will generate a larger executable (around 800 kb). On Ubuntu the toolchain can be installed with ```sudo apt install g++-mingw-w64-x86-64```.

		x86_64-w64-mingw32-g++ -o launcher.o -c launcher.cpp
		x86_64-w64-mingw32-windres resources.rc --output-format=coff --output=resources.res
		x86_64-w64-mingw32-g++ -o launcher.exe -static -s -mwindows launcher.o resources.res -lshlwapi

You will now have a file called **launcher.exe**, which you can rename to suit your own project.
