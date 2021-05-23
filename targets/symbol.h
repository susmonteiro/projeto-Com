#ifndef __FIR_TARGETS_SYMBOL_H__
#define __FIR_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace fir {

  class symbol {
    int _qualifier;
    std::shared_ptr<cdk::basic_type> _type;
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;

    std::string _name;
    long _value; // hack!
    bool _forward = false;

  public:
    symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, long value, bool forward = false) :
        _qualifier(qualifier), _type(type), _name(name), _value(value), _forward(forward) {
    }

    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, int qualifier) :
        _qualifier(qualifier), _type(type), _name(name) {
    }

    /* symbol(bool constant, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool initialized,
           bool function, bool forward = false) :
        _name(name), _value(0), _constant(constant), _qualifier(qualifier), _type(type), _initialized(initialized), _function(
            function), _forward(forward) { */

    virtual ~symbol() {
      // EMPTY
    }

    int qualifier() const {
      return _qualifier;
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
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

    bool forward() const {
      return _forward;
    }

    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }
  };

  inline auto make_symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name,
                          long value, bool forward = false) {
    return std::make_shared<symbol>(qualifier, type, name, value, forward);
  }

} // fir

#endif
