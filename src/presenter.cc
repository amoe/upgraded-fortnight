#include <iostream>
#include <stdexcept>
#include <exception>
#include "presenter.hh"
#include "settings.hh"
#include "logging.hh"
#include "slot_adapter.hh"
#include "xlsx.hh"
#include "mapping_scheme_serializer.hh"
#include "mapping_scheme_deserializer.hh"

using std::string;
using std::runtime_error;

void PresenterImpl::setView(View* view) {
    this->view = view;

    // By doing this here we install the progress adapter inside the driver.
    // When the driver runs, it will know to call back to the progress adapter,
    // which will then emit a signal that is handled by the GUI thread.
    // We also instruct the view to bind to the signal.
    
    this->progressReporter = new ViewProgressAdapter();
    driver->setProgressReporter(progressReporter);
    view->setProgressReporter(progressReporter);
}

void PresenterImpl::initializeView() {
    spdlog::info("initializing view");

    view->setToken(Settings::getTokenOrEmpty());
    view->setAvailableEncoders(model->getAvailableEncoders());
}

void PresenterImpl::startUpload() {
    try {
        spdlog::info("presenter slot was called");

        // This token will now be used by everything, because everything else
        // holds a reference to it.
        string token = view->getToken();
        tokenStore->setToken(token);
        Settings::setToken(token);

        string inputFile = view->getSelectedFile();
        spdlog::info("value of text input is {}", view->getSelectedFile());

        StringAdapter adapter(this, &Presenter::uploadFinished);
        StringAdapter errorAdapter(this, &Presenter::fatalError);

        uploadTask = unique_ptr<RunUploadTask>(new RunUploadTask(
            driver, adapter, errorAdapter, inputFile, model->getFieldMappings()
        ));
        uploadTask->run();
    // exceptions aren't polymorphic so we have to catch both
    } catch (std::runtime_error& e) {
        spdlog::info("caught runtime-error in gui thread handler: {}", e.what());
    } catch (std::exception& e) {
        spdlog::info("caught std-exception in gui thread handler: {}", e.what());
    }
}

void PresenterImpl::uploadFinished(string value) {
    spdlog::info("upload was finished: %s", value.c_str());
}

void PresenterImpl::fatalError(string what) {
    view->reportError(what);
}

void PresenterImpl::pickFile() {
    spdlog::info("file pick requested");
    bool fileWasPicked = view->showFileDialog();

    if (fileWasPicked) {
        view->addLog("Ready to upload.");
    }
}

void PresenterImpl::showAboutDialog() {
    spdlog::info("about dialog requested");
    view->showAboutDialog();
}

void PresenterImpl::showSettingsDialog() {
    spdlog::info("requested to show settings dialog");
    if (model->getSourceFile().has_value()) {
        // At this point, the source file has already been scanned.
        // We pass a reference to the field mappings stored inside the model.
        view->showSettingsDialog(
            model->getHeaderFields(),
            model->getFieldMappings(),
            model->getConverterRegistry()
        );
    } else {
        view->reportError("Please select an input file first.");
    }
}

void PresenterImpl::fileConfirmed(string fileName) {
    std::cout << "filename was " << fileName << std::endl;

    model->setSourceFile(fileName);

    // Because we know -- ASSUME -- that the model is a dumb memory-store,
    // we don't call additional accessor methods on the model, although we certainly
    // should.
    // i.e. -- The model might massage the value, but we assume here that it
    // doesn't.

    view->setSourceFile(fileName);
    view->addLog("Scanning sheet columns.");
    XlsxReader reader(fileName);
    vector<string> headerFields = reader.rowToString(1);
    model->setHeaderFields(headerFields);
    view->addLog("Scanned sheet headers.");
    model->dumpMappingScheme();
}


void PresenterImpl::settingsConfirmed() {
    spdlog::info("settings confirmed");
}

void PresenterImpl::fieldEncoderConfigurationDialogConfirmed(
    domain::FieldEncoderListOperation dto
) {
    spdlog::info("inside presenter fieldEncoderConfigurationDialogConfirmed");

    FieldEncoder newEncoderState = getEncoderFromDto(dto);

    if (dto.index >= 0) {
        std::cout << "I will replace an encoder." << std::endl;
        model->replaceFieldEncoder(dto.index, newEncoderState);
    } else {
        std::cout << "I will add an encoder." << std::endl;
        std::cout << "Found: " << newEncoderState.describe() << std::endl;
        model->addFieldEncoder(newEncoderState);
    }
}

FieldEncoder PresenterImpl::getEncoderFromDto(
    domain::FieldEncoderListOperation dto
) const {
    optional<TargetField> targetField = nullopt;
    if (dto.isTargetFieldSet) {
        targetField = TargetField(
            static_cast<TargetFieldType>(dto.targetFieldTypeId),
            dto.fieldName
        );
    }

    FieldEncoder newEncoder(
        targetField,
        static_cast<ConverterName>(dto.converterIndex),
        {},
        dto.newOptions
    );

    return newEncoder;
}

void PresenterImpl::onMappingEncoderSetOperation(
    domain::MappingEncoderSetOperation dto
) {
    spdlog::info("inside presenterimpl mappingencodersetoperation");

    // Take action based on what happened
    std::cout << "Excel row index was " << dto.excelRowIndex << std::endl;
    std::cout << "Field encoder index was " << dto.fieldEncoderIndex << std::endl;

    model->bindRow(dto.excelRowIndex, dto.fieldEncoderIndex);
}

void PresenterImpl::saveFieldMappings(string outputPath) {
    spdlog::info("presenter would save mappings");
    MappingSchemeSerializer serializer;
    serializer.saveMappingScheme(model->getFieldMappings(), outputPath);
    view->infoBox("Saved mappings successfully.");
}

void PresenterImpl::loadFieldMappings(string inputPath) {
    checkState();

    spdlog::info("presenter would load mappings");

    spdlog::info("init deserializer");
    MappingSchemeDeserializer deserializer;
    spdlog::info("request load");
    MappingScheme newMappings = deserializer.loadMappingScheme(inputPath);
    spdlog::info("request replace");
    model->replaceFieldMappings(newMappings);
    spdlog::info("request refresh");
    view->forceRefreshFieldMappings();
}

void PresenterImpl::checkState() const {
    if (view == nullptr) {
        throw runtime_error("view was not initialized");
    }
}
