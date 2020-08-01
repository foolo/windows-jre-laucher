This directory contains a sample application that can be bundled together with a JRE, using the commands below.

Compile the application and create the JAR.
```
javac com/example/MyApplication.java
jar cvfe MyApplication.jar com.example.MyApplication com/example/*
```

Create a bundled JRE with the *jlink* tool:
```
jlink --add-modules java.base,java.desktop --output jre
```

You can now run the application by starting Java from the JRE folder:

```
jre/bin/java com.example.MyApplication
```
