#include "DataProperty.h"

#include <autordf/cvt/Cvt.h>

#include "Klass.h"
#include "Utils.h"

namespace autordf {
namespace codegen {

std::map<cvt::RdfTypeEnum, std::string> rdf2CppTypeMapping(
        {
//FIXME: handle PlainLiteral, XMLLiteral, Literal, real, rational
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_string,             "std::string"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_boolean,            "bool"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_decimal,            "double"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_float,              "float"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_double,             "double"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_dateTime,           "boost::posix_time::ptime"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_integer,            "long long int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_dateTimeStamp,      "boost::posix_time::ptime"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_nonNegativeInteger, "long long unsigned int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_positiveInteger,    "long long unsigned int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_nonPositiveInteger, "long long int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_negativeInteger,    "long long int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_long,               "long long int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_unsignedLong,       "unsigned long long int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_int,                "long int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_unsignedInt,        "long unsigned int"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_short,              "short"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_unsignedShort,      "unsigned short"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_byte,               "char"),
        std::pair<cvt::RdfTypeEnum, std::string>(cvt::RdfTypeEnum::xsd_unsignedByte,       "unsigned char")
    }
);

void DataProperty::generateDeclaration(std::ostream& ofs, const Klass& onClass) const {
    indent(ofs, 1) << "/**" << std::endl;
    indent(ofs, 1) << " * Full iri for " <<  _decorated.rdfname().prettyName() << " data property." << std::endl;
    indent(ofs, 1) << " */" << std::endl;
    indent(ofs, 1) << "static const autordf::Uri " << _decorated.prettyIRIName() << "DataPropertyIri;" << std::endl;
    ofs << std::endl;

    if ( _decorated.maxCardinality(onClass.decorated()) <= 1 ) {
        if ( _decorated.minCardinality(onClass.decorated()) > 0 ) {
            generateGetterForOneMandatory(ofs, onClass);
        } else {
            generateGetterForOneOptional(ofs, onClass);
        }
        ofs << std::endl;
        generateSetterForOne(ofs, onClass);
    }
    if ( _decorated.maxCardinality(onClass.decorated()) > 1 ) {
        generateGetterForMany(ofs, onClass);
        ofs << std::endl;
        generateSetterForOne(ofs, onClass);
        ofs << std::endl;
        generateSetterForMany(ofs, onClass);
    }
    if ( _decorated.minCardinality(onClass.decorated()) != _decorated.maxCardinality(onClass.decorated()) ) {
        ofs << std::endl;
        generateRemover(ofs, onClass);
    }
}

void DataProperty::generateKeyDeclaration(std::ostream& ofs, const Klass& onClass) const {
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);
    std::string currentClassName = onClass.decorated().prettyIRIName();

    indent(ofs, 1) << "/**" << std::endl;
    indent(ofs, 1) << " * @brief This method " << genCppNameSpaceFullyQualified() << "::" << currentClassName << "::findBy" << _decorated.prettyIRIName(true) << " returns the only instance of " + onClass.decorated().rdfname() + " with property " + _decorated.rdfname() +  " set to given value." << std::endl;
    indent(ofs, 1) << " * " << std::endl;
    indent(ofs, 1) << " * @param key value that uniquely identifies the expected object" << std::endl;
    indent(ofs, 1) << " * " << std::endl;
    indent(ofs, 1) << " * @throw DuplicateObject if more than one object have the same property value" << std::endl;
    indent(ofs, 1) << " * @throw ObjectNotFound if no object has given property with value" << std::endl;
    indent(ofs, 1) << " */" << std::endl;

    indent(ofs, 1) << "static " << currentClassName << " findBy" << _decorated.prettyIRIName(true) << "( const autordf::PropertyValue& key ) {" << std::endl;
    indent(ofs, 2) <<     "return Object::findByKey(\"" << _decorated.rdfname() << "\", key).as<" << currentClassName << ">();" << std::endl;
    indent(ofs, 1) << "}" << std::endl;
    indent(ofs, 1) << std::endl;

    if (!rdfCppType.second.empty()) {
        indent(ofs, 1) << "/**" << std::endl;
        indent(ofs, 1) << " * @brief This method " << genCppNameSpaceFullyQualified() << "::" << currentClassName << "::findBy" << _decorated.prettyIRIName(true) << " returns the only instance of " + onClass.decorated().rdfname() + " with property " + _decorated.rdfname() +  " set to given value." << std::endl;
        indent(ofs, 1) << " * " << std::endl;
        indent(ofs, 1) << " * @param key value that uniquely identifies the expected object" << std::endl;
        indent(ofs, 1) << " * " << std::endl;
        indent(ofs, 1) << " * @throw DuplicateObject if more than one object have the same property value" << std::endl;
        indent(ofs, 1) << " * @throw ObjectNotFound if no object has given property with value" << std::endl;
        indent(ofs, 1) << " */" << std::endl;

        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << "static " << currentClassName << " findBy" << _decorated.prettyIRIName(true) << "( const " << cppType << "& key ) {" << std::endl;
        indent(ofs, 2) <<     "return findBy" << _decorated.prettyIRIName(true) << "(autordf::PropertyValue().set<autordf::cvt::RdfTypeEnum::" << cvt::rdfTypeEnumString(rdfCppType.first) << ">(key));" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
        indent(ofs, 1) << std::endl;
    }
}

void DataProperty::generateDefinition(std::ostream& ofs, const Klass& onClass) const {
    std::string currentClassName = "I" + onClass.decorated().prettyIRIName();
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);

    ofs << "const autordf::Uri " << currentClassName << "::" << _decorated.prettyIRIName() << "DataPropertyIri = \"" << _decorated.rdfname() << "\";" << std::endl;
    ofs << std::endl;

    if (!rdfCppType.second.empty()) {
        cvt::RdfTypeEnum rdfType = rdfCppType.first;
        std::string cppType = rdfCppType.second;


        if (_decorated.maxCardinality(onClass.decorated()) <= 1) {
            if (_decorated.minCardinality(onClass.decorated()) > 0) {
                // Nothing
            } else {
                ofs << "std::shared_ptr<" << cppType << "> " << currentClassName << "::" << _decorated.prettyIRIName() << "Optional() const {" << std::endl;
                indent(ofs, 1) << "auto ptrval = object().getOptionalPropertyValue(\"" << _decorated.rdfname() << "\");" << std::endl;
                indent(ofs, 1) << "return (ptrval ? std::shared_ptr<" << cppType << ">(new " << cppType <<
                "(ptrval->get<autordf::cvt::RdfTypeEnum::" << cvt::rdfTypeEnumString(rdfType) << ", " << cppType << ">())) : nullptr);" << std::endl;
                ofs << "}" << std::endl;
                ofs << std::endl;
                ofs << cppType << " " << currentClassName << "::" << _decorated.prettyIRIName() << "(const " << cppType <<"& defaultval) const {" << std::endl;
                indent(ofs, 1) << "auto ptrval = object().getOptionalPropertyValue(\"" << _decorated.rdfname() << "\");" << std::endl;
                indent(ofs, 1) << "return (ptrval ? ptrval->get<autordf::cvt::RdfTypeEnum::" << cvt::rdfTypeEnumString(rdfType) << ", " << cppType << ">() : defaultval);" << std::endl;
                ofs << "}" << std::endl;
            }
        }
        if (_decorated.maxCardinality(onClass.decorated()) > 1) {
            ofs << "std::vector<" << cppType << "> " << currentClassName << "::" << _decorated.prettyIRIName() <<
            "List() const {" << std::endl;
            indent(ofs, 1) << "return object().getValueListImpl<autordf::cvt::RdfTypeEnum::" <<
            cvt::rdfTypeEnumString(rdfType) << ", " << cppType << ">(\"" << _decorated.rdfname() << "\", " << orderedBoolValue() << ");" << std::endl;
            ofs << "}" << std::endl;
            ofs << std::endl;
            ofs << currentClassName << "& " << currentClassName << "::set" << _decorated.prettyIRIName(true) << "(const std::vector<" << cppType << ">& values) {" << std::endl;
            indent(ofs, 1) <<     "object().setValueListImpl<autordf::cvt::RdfTypeEnum::" <<
            cvt::rdfTypeEnumString(rdfType) << ">(\"" << _decorated.rdfname() << "\", values, " << orderedBoolValue() << ");" << std::endl;
            indent(ofs, 1) << "return *this;" << std::endl;
            ofs << "}" << std::endl;
        }
    }
}

std::pair<cvt::RdfTypeEnum, std::string> DataProperty::getRdfCppTypes(const Klass& onClass) const {
    auto rdfTypeEntry = cvt::rdfMapType.find(_decorated.range(&onClass.decorated()));

    if (rdfTypeEntry != cvt::rdfMapType.end()) {
        auto cppTypeEntry = rdf2CppTypeMapping.find(rdfTypeEntry->second);
        if (cppTypeEntry != rdf2CppTypeMapping.end()) {
            return std::make_pair(rdfTypeEntry->second, cppTypeEntry->second);
        }
    }
    return std::make_pair(cvt::RdfTypeEnum::xsd_string , "");
}

void DataProperty::generateGetterForOneMandatory(std::ostream& ofs, const Klass& onClass) const {
    std::string methodName = _decorated.prettyIRIName();
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "@return the mandatory value for this property.\n"
                            "@throw PropertyNotFound if value is not set in database");
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);

    if (!rdfCppType.second.empty()) {
        cvt::RdfTypeEnum rdfType = rdfCppType.first;
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << cppType << " " << methodName << "() const {" << std::endl;
        indent(ofs, 2) << "return object().getPropertyValue(\"" << _decorated.rdfname() << "\").get<autordf::cvt::RdfTypeEnum::" << cvt::rdfTypeEnumString(rdfType) << ", " << cppType<< ">();" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    } else {
        indent(ofs, 1) << "autordf::PropertyValue " << methodName << "() const {" << std::endl;
        indent(ofs, 2) << "return object().getPropertyValue(\"" << _decorated.rdfname() << "\");" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }
}

void DataProperty::generateSetterForOne(std::ostream& ofs, const Klass& onClass) const {
    std::string methodName = "set" + _decorated.prettyIRIName(true);
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "Sets the mandatory value for this property.\n"
                            "@param value value to set for this property, removing all other values");
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);

    std::string currentClassName = "I" + onClass.decorated().prettyIRIName();

    if (!rdfCppType.second.empty()) {
        cvt::RdfTypeEnum rdfType = rdfCppType.first;
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << currentClassName << "& " << methodName << "(const " << cppType << "& value) {" << std::endl;
        indent(ofs, 2) << "object().setPropertyValue(\"" << _decorated.rdfname() <<
            "\", autordf::PropertyValue().set<autordf::cvt::RdfTypeEnum::" << cvt::rdfTypeEnumString(rdfType) <<
            ">(value));" << std::endl;
        indent(ofs, 2) << "return *this;" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    } else {
        indent(ofs, 1) << currentClassName << "& set" << _decorated.prettyIRIName(true) << "(const autordf::PropertyValue& value) {" << std::endl;
        indent(ofs, 2) << "object().setPropertyValue(\"" << _decorated.rdfname() << "\", value);" << std::endl;
        indent(ofs, 2) << "return *this;" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }
}

void DataProperty::generateGetterForOneOptional(std::ostream& ofs, const Klass& onClass) const {
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);

    std::string methodName = _decorated.prettyIRIName() + "Optional";
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "@return the valueif it is set, or nullptr if it is not set.");

    if (!rdfCppType.second.empty()) {
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << "std::shared_ptr<" << cppType << "> " << methodName << "() const;" << std::endl;
    } else {
        indent(ofs, 1) << "std::shared_ptr<autordf::PropertyValue> " << methodName << "() const {" << std::endl;
        indent(ofs, 2) << "return object().getOptionalPropertyValue(\"" << _decorated.rdfname() << "\");" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }

    ofs << std::endl;
    methodName = _decorated.prettyIRIName();
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "@return the valueif it is set, or defaultval if it is not set.");

    if (!rdfCppType.second.empty()) {
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << cppType << " " << _decorated.prettyIRIName() << "(const " << cppType << "& defaultval) const;" << std::endl;
    } else {
        indent(ofs, 1) << "autordf::PropertyValue " << _decorated.prettyIRIName() << "(const autordf::PropertyValue& defaultval) const {" << std::endl;
        indent(ofs, 2) << "auto ptr = object().getOptionalPropertyValue(\"" << _decorated.rdfname() << "\");" << std::endl;
        indent(ofs, 2) << "return (ptr ? *ptr : defaultval);" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }
}

void DataProperty::generateGetterForMany(std::ostream& ofs, const Klass& onClass) const {
    std::string methodName = _decorated.prettyIRIName() + "List";
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "@return the list of values.  List can be empty if not values are set in database");
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);

    if (!rdfCppType.second.empty()) {
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << "std::vector<" << cppType << "> " << methodName << "() const;" << std::endl;
    } else {
        indent(ofs, 1) << "autordf::PropertyValueVector " << methodName << "() const {" << std::endl;
        indent(ofs, 2) <<     "return object().getPropertyValueList(\"" << _decorated.rdfname() << "\", " << orderedBoolValue() << ");" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }
}

void DataProperty::generateSetterForMany(std::ostream& ofs, const Klass& onClass) const {
    std::string methodName = "set" + _decorated.prettyIRIName(true);
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "Sets property to list of values \n@param values the list of values");
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);

    std::string currentClassName = "I" + onClass.decorated().prettyIRIName();

    if (!rdfCppType.second.empty()) {
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << currentClassName << "& " << methodName << "(const std::vector<" << cppType << "> " << "& values);" << std::endl;
    } else {
        indent(ofs, 1) << currentClassName << "& " << methodName << "(const autordf::PropertyValueVector& values) {" << std::endl;
        indent(ofs, 2) <<     "object().setPropertyValueList(\"" << _decorated.rdfname() << "\", values, " << orderedBoolValue() << ");" << std::endl;
        indent(ofs, 2) <<     "return *this;" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }
    ofs << std::endl;

    methodName = "add" + _decorated.prettyIRIName(true);
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "Adds a value to a property \n@param value the value to add");
    if (!rdfCppType.second.empty()) {
        cvt::RdfTypeEnum rdfType = rdfCppType.first;
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << currentClassName << "& " << methodName << "(const " << cppType << "& value) {" << std::endl;
        indent(ofs, 2) << "object().addPropertyValue(\"" << _decorated.rdfname() <<
        "\", autordf::PropertyValue().set<autordf::cvt::RdfTypeEnum::" << cvt::rdfTypeEnumString(rdfType) <<
        ">(value), " << orderedBoolValue() << ");" << std::endl;
        indent(ofs, 2) << "return *this;" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    } else {
        indent(ofs, 1) << currentClassName << "& " << methodName << "(const autordf::PropertyValue& value) {" << std::endl;
        indent(ofs, 2) << "object().addPropertyValue(\"" << _decorated.rdfname() << "\", value, " << orderedBoolValue() << ");" << std::endl;
        indent(ofs, 2) << "return *this;" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }
}

void DataProperty::generateRemover(std::ostream& ofs, const Klass& onClass) const {
    std::string methodName = "remove" + _decorated.prettyIRIName(true);
    generatePropertyComment(ofs, onClass, methodName, 1,
                    "Remove a value for this property.\n"
                            "@param value to remove for this property.\n"
                            "@throw PropertyNotFound if propertyIRI has not obj as value\n");
    std::pair<cvt::RdfTypeEnum, std::string> rdfCppType = getRdfCppTypes(onClass);

    std::string currentClassName = "I" + onClass.decorated().prettyIRIName();

    if (!rdfCppType.second.empty()) {
        cvt::RdfTypeEnum rdfType = rdfCppType.first;
        std::string cppType = rdfCppType.second;
        indent(ofs, 1) << currentClassName << "& " << methodName << "(const " << cppType << "& value) {" << std::endl;
        indent(ofs, 2) << "object().removePropertyValue(\"" << _decorated.rdfname() <<
        "\", autordf::PropertyValue().set<autordf::cvt::RdfTypeEnum::" << cvt::rdfTypeEnumString(rdfType) <<
        ">(value));" << std::endl;
        indent(ofs, 2) << "return *this;" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    } else {
        indent(ofs, 1) << currentClassName << "& " << methodName << "(const autordf::PropertyValue& value) {" << std::endl;
        indent(ofs, 2) << "object().removePropertyValue(\"" << _decorated.rdfname() << "\", value);" << std::endl;
        indent(ofs, 2) << "return *this;" << std::endl;
        indent(ofs, 1) << "}" << std::endl;
    }
}

std::string DataProperty::orderedBoolValue() const {
    return _decorated.ordered() ? "true" : "false";
}

}
}
