//
// Created by sbaguet on 10/28/15.
//

#ifndef AUTORDF_URI_H
#define AUTORDF_URI_H

#include <string>

namespace autordf {

class Model;

/**
 * An Uri: will contain in the future URi manipulation methods
 */
class Uri : public std::string {
public:
    /**
     * Base class constructor inheritance
     */
    using std::string::string;

    /**
     * Constructor to make Uri from string.
     * Should be automatically done by above declaration but not working
     */
    Uri(const std::string& str = "") : std::string(str) {};

    /**
     * Returns local part of uri
     * e.g. http://my/complex/path/to/dog --> dog
     * e.g. my:attribute --> attribute
     */
    std::string localPart() const;

    /**
     * Display friendly name: takes last part of IRI and make it something short and understandable for
     * human begin.
     */
    std::string prettyName(bool uppercaseFirst = false) const;

    /**
     * QName for this iri.
     * Known prefixes are used from model
     *
     * @return QName
     */
    std::string QName(const Model *model = nullptr) const;
};

}


#endif //AUTORDF_URI_H
