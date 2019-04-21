#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "optional.hpp"
#include "mapping_engine.hh"
#include "mapping_types.hh"
#include "test_utility.hh"
#include "test_vocabulary.hh"
#include "default_field_encoders.hh"

using nonstd::optional;
using nonstd::nullopt;
using std::vector;
using std::string;

class MappingEngineTest: public Test {
public:
    MappingEngineTest() {
        lookups = new LookupRegistryImpl;
        engine = new MappingEngine(lookups);
        
    }
    ~MappingEngineTest() {
        // It can be this that triggers the problem, because it's deallocating
        // this pointer.  However, this is allocated with new, so no reason
        // for it to happen really.
        delete lookups;
        delete engine;
    }

    LookupRegistry* lookups;
    MappingEngine* engine;
};

// It's quite difficult to construct these MappingSchemes in C++ code.
// This isn't to be construed as a feature, but these structures are the full
// definition of the available behaviours.
// There might be more 'sugary' ways to write them that could be constructed
// later.

TEST_F(MappingEngineTest, TitleStringConversionCheck) {
    MappingScheme theScheme = {default_field_encoders::TITLE_ENCODER};

    vector<string> theDocument = {"foo"};

    MappingOutput result = this->engine->convert(theDocument, theScheme);

    const string expectedResult = R"(
        {
           "title": "foo"
        }
    )";

    vector<string> expectedSourcePaths = {};

    ASSERT_THAT(result.getArticleObject(), Eq(deserialize(expectedResult)));
    ASSERT_THAT(result.getSourcePaths(), Eq(expectedSourcePaths));
}


TEST_F(MappingEngineTest, ContributeFilesCheck) {
    vector<string> theDocument = {
        "foo.tiff;bar.tiff"
    };
    OptionsMap options = {
        {"delimiter", optional<string>(";")}
    };

    FieldEncoder contributeFilesEncoder(
        nullopt,
        ConverterName::CONTRIBUTE_FILES,
        {},
        options
    );
    MappingScheme theScheme = {contributeFilesEncoder};

    MappingOutput result = this->engine->convert(theDocument, theScheme);

    // Expect an empty article object because we haven't defined any other
    // converters.
    QJsonObject expectedArticle;
    vector<string> expectedSourcePaths = {
        "foo.tiff", "bar.tiff"
    };

    ASSERT_THAT(result.getArticleObject(), Eq(expectedArticle));
    ASSERT_THAT(result.getSourcePaths(), Eq(expectedSourcePaths));
}


TEST_F(MappingEngineTest, DefinedTypeLookupListCheck) {
    vector<string> theDocument = {
        "Figure"
    };
    OptionsMap options = {
        {"resourceName", optional<string>("definedType")}
    };


    TargetField targetField(TargetFieldType::STANDARD, "defined_type");
    FieldEncoder lookupListEncoder(
        optional<TargetField>(targetField),
        ConverterName::LOOKUP_LIST,
        {},
        options
    );
    MappingScheme theScheme = {lookupListEncoder};

    MappingOutput result = this->engine->convert(theDocument, theScheme);

    // Expect an empty article object because we haven't defined any other
    // converters.
    QJsonObject expectedArticle;
    vector<string> expectedSourcePaths = {};

    const string expectedResult = R"(
        {
           "defined_type": "figure"
        }
    )";


    ASSERT_THAT(result.getArticleObject(), Eq(deserialize(expectedResult)));
    ASSERT_THAT(result.getSourcePaths(), Eq(expectedSourcePaths));
}

TEST_F(MappingEngineTest, DiscardConverterCheck) {
    MappingScheme theScheme = {default_field_encoders::DISCARD_ENCODER};
    vector<string> theDocument = {"foo"};

    MappingOutput result = this->engine->convert(theDocument, theScheme);

    const string expectedResult = R"(
        {
        }
    )";
    vector<string> expectedSourcePaths = {};

    ASSERT_THAT(result.getArticleObject(), Eq(deserialize(expectedResult)));
    ASSERT_THAT(result.getSourcePaths(), Eq(expectedSourcePaths));
}

TEST_F(MappingEngineTest, KeywordEncoderCheck) {
    MappingScheme theScheme = {default_field_encoders::KEYWORDS_ENCODER};
    vector<string> theDocument = {"foo, bar, baz"};

    MappingOutput result = this->engine->convert(theDocument, theScheme);

    const string expectedResult = R"(
        {
            "keywords": ["foo", "bar", "baz"]
        }
    )";
    vector<string> expectedSourcePaths = {};

    ASSERT_THAT(result.getArticleObject(), Eq(deserialize(expectedResult)));
    ASSERT_THAT(result.getSourcePaths(), Eq(expectedSourcePaths));
}

