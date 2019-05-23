#ifndef __M19_SEMANTICS_SYMBOL_H__
#define __M19_SEMANTICS_SYMBOL_H__

#include <string>
#include <list>
#include <cdk/basic_type.h>

namespace m19 {

  class symbol {
    std::string _name;
    long _value; // hack! ????
    int _qualifier;
    basic_type *_type;

    bool _initialized;
    int _offset = 0; // 0 (zero) means global variable/function
    bool _function; // false for variables
    std::list<std::shared_ptr<symbol>> _args; //arguments of function
    bool _forward = false;

  public:
    symbol(int qualifier, basic_type *type, const std::string &name, bool initialized, bool function, bool forward =
               false) :
        _name(name), _value(0), _qualifier(qualifier), _type(type), _initialized(initialized), _function(
            function), _forward(forward) {
    }

    virtual ~symbol() {
      delete _type;
    }

    basic_type *type() const {
      return _type;
    }

    const std::string &name() const {
      return _name;
    }

    long value() const {
      return _value;
    }

    long value(long v) {
      return _value = v;
    }

    int qualifier() const {
      return _qualifier;
    }

    bool initialized() const {
      return _initialized;
    }

    int offset() const {
      return _offset;
    }

    void set_offset(int offset) {
      _offset = offset;
    }

    bool isFunction() const {
      return _function;
    }

    bool global() const {
      return _offset == 0;
    }

    bool isVariable() const {
      return !_function;
    }

    bool forward() const {
      return _forward;
    }

    std::list<std::shared_ptr<symbol>> *args() {
      return &_args;
    }
  };

} // m19

#endif
