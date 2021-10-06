

<h1 align="center">JVM Native Class Dumper</h1>
<div align="center">
<img src="https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat" alt="Build: passing">
<img src="https://img.shields.io/badge/license-MPL%202.0-green.svg?style=flat" alt="License: MPL">
<a href="https://discord.gg/b2AUPaj"><img src="https://img.shields.io/badge/chat%20on-discord-7289da.svg?style=flat" alt="discord"></a>

</div>
<hr>
<p>Simple native jvm class dumper written in C by hook ClassLoader</p>

## What is used for?

This tool allows you to dump all java classes passed through the ClassLoader. If app decrypt or modify their classes at runtime and there's no easy way to decrypt them or view statically, you can just use this tool and dump decrypted classes from memory.<br />
All classes are dumped into location "C:\\JavaClassDump\\"


## Building

### Visual Studio
Simplest way to build project. Just import or clone repo and click "Build". Simple as that.

### Command line
Just run `msbuild /m /p:Configuration=Release .`

## License

[MPL-2.0](https://github.com/TheEasyPeasy/Java-Runtime-ClassDumper/blob/master/LICENSE)

Copyright (c) 2021-present, TheEasyPeasy
