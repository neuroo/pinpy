# PinPy - Shake your hooks with some Python!

Disclaimer: The code is essentially a proof of concept developed by Romain Gaucher ([@rgaucher](https://twitter.com/rgaucher)) and Haris Andrianakis ([@tl0gic](https://twitter.com/tl0gic)) . 
            It was tested on example programs, and larger. For larger ones, well... it will slow down the execution.
            Even if PinPy is not under active development, we decided to release, since the
            code might interest few folks out there.

PinPy essentially allows you to to run Python scripts inside whatever programs, just educate it
using the XML config file, what you want to hook. You'll need to get some information so that
pin can properly install the hooks such as signature, etc. This is done by embedding a Python runtime
in a pin tool.

Technically, we hook the selected functions using Pin, and redirect the execution flow to the 
selected Python function. The XML config allows you to specify when you want to hook the function
call (for instance, before the function, after, etc.).

# Example
Configuration for Pidgin, we want to intercept log the SSL traffic:

    <PinPyFunction name="purple_ssl_read_wrapper">
        <FunctionPrototype>
            <FunctionName>purple_ssl_read</FunctionName>
            <FunctionParameters>
                <FunctionParameter id="0">
                    <FunctionParameterName>gsc</FunctionParameterName>
                    <FunctionParameterType>int</FunctionParameterType>
                </FunctionParameter>
                <FunctionParameter id="1">
                    <FunctionParameterName>data</FunctionParameterName>
                    <FunctionParameterType>char *</FunctionParameterType>
                </FunctionParameter>
                <FunctionParameter id="2">
                    <FunctionParameterName>len</FunctionParameterName>
                    <FunctionParameterType>int</FunctionParameterType>
                </FunctionParameter>
            </FunctionParameters>
            <FunctionReturnType>int</FunctionReturnType>
        </FunctionPrototype>
        <BeforeCall>
            <PinPyActions>
                <PinPyAction name="Logging before purple_ssl_read" sequence="1">
                    <PyCall module="sample" function="log">
                        <Message>purple_ssl_read -- BeforeCall</Message>
                     </PyCall>
                </PinPyAction>
            </PinPyActions>
        </BeforeCall>
        <AfterCall>
            <PinPyActions>
                <PinPyAction name="Logging after purple_ssl_read" sequence="1">
                    <PyCall module="sample" function="log">
                        <Message>purple_ssl_read -- AfterCall</Message>
                     </PyCall>
                </PinPyAction>
            </PinPyActions>
        </AfterCall>
    </PinPyFunction>

In this case, we decide to intercept all calls to `purple_ssl_read_wrapper`, and redirect them
to our logging function `log` implemented in `sample.py`. 

This logging function `log` is called from the Pin tool, and the parameters will be passed to it:

    # ... helpers functions for ctype available in the Python script
    #     in the example
    def log(a_dict):
        if a_dict["@callMode"] == "beforeCall":
            if a_dict["@funcName"] == "purple_ssl_read":
                data = ''
                gsc  = length = -1
                if a_dict.has_key("gsc"):
                    gsc = readInt(a_dict["gsc"])
                if a_dict.has_key("data"):
                    data = readStr(a_dict["data"])
                if a_dict.has_key("len"):
                    length = readInt(a_dict["len"])
                print length, data.size()
# Build & hack on it
Using `scons`, or just tweak the `Makefile`!

# Links
- [Pin](http://www.pintool.org/), for dynamic instrumentation of binaries
