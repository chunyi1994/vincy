# vincy
vincy是一个c++ web框架. python-flask和go语言的web框架都非常简洁大方,所以我开发了一个相同用法的框架

这个框架使用非常简单, main.cpp里面提供了一个例子.

这个框架从tcp层开始接管,底层使用boost.asio,所以不可以和nginx同时使用,也不是基于cgi的.

这个框架自带一个模板引擎,用法和jinja应该是差不多的.
