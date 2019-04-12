#ifndef CONVERTER_REGISTRY_HH
#define CONVERTER_REGISTRY_HH

#include <unordered_map>
#include <QJsonValue>
#include "mapping_types.hh"

using std::unordered_map;

// This is basically a hack to allow storing enum classes in an unordered_map,
// and it should definitely be moved somewhere more useful.


struct EnumClassHash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};


class Converter {
public:
    virtual IntermediateMappingOutput applyConversion(string input, OptionsMap options) = 0;
};

class StringConverter: public Converter {
public:
    IntermediateMappingOutput applyConversion(string input, OptionsMap options);
};

class ConverterRegistry {
public:
    ConverterRegistry();
    ~ConverterRegistry();

    // This should really be static, but we don't know any sensible way to
    // access state in this case.
    IntermediateMappingOutput convert(ConverterName c, string value, OptionsMap options);

private:
    unordered_map<ConverterName, Converter*, EnumClassHash> converterMap;
};

#endif /* CONVERTER_REGISTRY_HH */
