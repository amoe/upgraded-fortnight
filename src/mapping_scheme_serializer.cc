#include <QJsonArray>
#include "mapping_scheme_serializer.hh"

QJsonObject MappingSchemeSerializer::serialize(MappingScheme scheme) const {
    QJsonObject result;
    QJsonArray rowContent;

    for (int i = 0; i < scheme.size(); i++) {
        int sourceRowIndex = i;
        QJsonObject rowMappingObject;
        FieldEncoder encoder = scheme.at(sourceRowIndex);

        rowMappingObject.insert("sourceRowIndex", sourceRowIndex);
        rowMappingObject.insert("fieldEncoder", serializeFieldEncoder(encoder));
        rowContent.push_back(rowMappingObject);
    }

    result.insert("rows", rowContent);

    return result;
}


QJsonValue MappingSchemeSerializer::serializeFieldEncoder(FieldEncoder f) const {
    QJsonObject result;

    result.insert("targetField", serializeTargetField(f.getTargetField()));
    result.insert("name", mapConverterName(f.getConverterName()));
    result.insert("validationRules", serializeValidationRules(f.getValidationRules()));
    result.insert("options", QJsonObject());

    return result;
}

QJsonValue MappingSchemeSerializer::serializeTargetField(optional<TargetField> t) const {
    QJsonObject result;

    if (t.has_value()) {
        TargetField field = t.value();
        result.insert("fieldType", mapTargetFieldType(field.getTargetFieldType()));
        result.insert("name", QJsonValue(QString::fromStdString(field.getName())));
    }

    return result;
}

QJsonValue MappingSchemeSerializer::mapTargetFieldType(TargetFieldType t) const {
    switch (t) {
        case TargetFieldType::STANDARD:
            return QJsonValue("standard");
            break;
        case TargetFieldType::CUSTOM:
            return QJsonValue("custom");
            break;
        default:
            throw new std::runtime_error("unknown target field type value");
    }
}

QJsonValue MappingSchemeSerializer::mapConverterName(ConverterName c) const {
    switch (c) {
        case ConverterName::STRING:
            return QJsonValue("string");
            break;
        default:
            throw new std::runtime_error("unknown converter name value");
    }
}

QJsonValue MappingSchemeSerializer::serializeValidationRules(
    vector<ValidationRule> rules
) const {
    QJsonArray result;
    return result;
}
