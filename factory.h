#ifndef __M19_FACTORY_H__
#define __M19_FACTORY_H__

#include <memory>
#include <cdk/yy_factory.h>
#include "m19_scanner.h"

namespace m19 {

  /**
   * This class implements the compiler factory for the Simple compiler.
   */
  class factory: public cdk::yy_factory<m19_scanner> {
    /**
     * This object is automatically registered by the constructor in the
     * superclass' language registry.
     */
    static factory _self;

  protected:
    /**
     * @param language name of the language handled by this factory (see .cpp file)
     */
    factory(const std::string &language = "m19") :
        cdk::yy_factory<m19_scanner>(language) {
    }

  };

} // m19

#endif
