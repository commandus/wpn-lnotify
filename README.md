# Web push notification command line interface client libnotify shared library

libwpn-lnotify.so

[Project description](https://docs.google.com/document/d/19pM4g-hvx2MUVV2Ggljw1MMTg9tMCEH7aHYYKpJMaWk/edit?usp=sharing)

## Dependencies

- libnotify 1.1.0 https://github.com/GNOME/libnotify/

## Tools

- Autoconf 2.63
- CMake 3.1 or higher

## Building

C++11 compliant compiler required.

```
cd wpn-lnotify
./autogen.sh
./configure
make
```

If you want, install:

```
sudo make install
```

## License

This software is licensed under the MIT License:

Copyright © 2018 Andrei Ivanov

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This software depends on libraries which has differen licenses:

- libnotify is licensed under GNU Lesser General Public; either version 2.1 of the License, or (at your option) any later version. LGPL-2.1


## Test

```
LD_PRELOAD=./libwpm-lnotify.so ./wpn
```