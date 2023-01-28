# 🌍 tcp-echo-server

This project is a simple implementation of a TCP echo server using BSD sockets that I put together to explore different methods of synchronous I/O multiplexing in a Linux environment.

It allows for the selection of a multiplexing method using an application argument. Available methods are select, poll, and epoll, and each method has performance and cross-platform compatibility trade-offs.

*Note*: this program uses epoll, a Linux kernel system call, so it can only be compiled in a Linux environment.

### Usage
```
echo_server 8080 epoll
```
- `arg[1]` the port you want the server to bind to.
- `arg[2]` the multiplexing method you want to use (select, poll, or epoll).

Both areguments are required.

## MIT licence
```
Copyright (c) 2023 Shlomi Nissan

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
