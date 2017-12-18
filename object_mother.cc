#include "object_mother.hh"
#include "ext/optional.hpp"

using nonstd::optional;
using nonstd::nullopt;

ArticleCreationRequest ObjectMother::makeArticleCreationRequest() {
    vector<string> keywords;
    keywords.push_back("Bethlehem");
    keywords.push_back("Crafts");

    vector<string> references;
    references.push_back("https://www.loc.gov/item/mpc2004001373/PP/");

    vector<int> categories;
    categories.push_back(1703);

    vector<string> authors;
    authors.push_back("Freja Howat-Maxted");

    ArticleCreationRequest request(
        "To Serve Man",
        "Some description",
        keywords,
        references,
        categories,
        authors,
        nullopt,
        ArticleType::FIGURE,
        1
    );

    return request;
}

FileInfo ObjectMother::makeFileInfo() {
    string uploadContainerUrl = "http://www.example.com/";
    string fileName = "nonexistent.jpg";
    int64_t id = 12345;
    FileInfo result(uploadContainerUrl, fileName, id);

    return result;
}

UploadContainerInfo ObjectMother::makeUploadContainerInfo(int nParts) {
    string name;
    int64_t size;
    string md5;
    string token;
    UploadContainerStatus status;
    vector<FilePart> parts;

    // 1-based loop!
    for (int i = 1; i < (nParts + 1); i++)
        parts.push_back(ObjectMother::makeFilePart(i));

    UploadContainerInfo result(name, size, md5, token, status, parts);

    return result;
}

FilePart ObjectMother::makeFilePart(int i) {
    int partNumber = i;
    int64_t startOffset = i * 10;
    int64_t endOffset = i * 20;
    bool isLocked = false;
    FilePartStatus status = FilePartStatus::PENDING;
    FilePart partSpec(partNumber, startOffset, endOffset, isLocked, status);
}
