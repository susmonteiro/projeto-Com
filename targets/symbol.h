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
    bool _initialized;
    int _offset = 0;
    bool _function;
    bool _forward = false;

  public:
    symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool initialized, bool function, bool forward = false) :
        _qualifier(qualifier), _type(type), _name(name), _initialized(initialized), _function(function), _forward(forward) {
    }

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

    int offset() const {
      return _offset;
    }

    void set_offset(int offset) {
      _offset = offset;
    }

    bool global() const {
      return _offset == 0;
    }

    bool isFunction() const {
      return _function;
    }

    bool forward() const {
      return _forward;
    }

    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }

    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const {
      return _argument_types.size();
    }
  };

  inline auto make_symbol(int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name,
                          bool initialized, bool function, bool forward = false) {
    return std::make_shared<symbol>(qualifier, type, name, initialized, function, forward);
  }

} // fir

#endif
