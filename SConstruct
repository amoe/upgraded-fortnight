import os
import subprocess

googletest_framework_root = "./ext/googletest-release-1.8.0"

googletest_include_paths = [
    googletest_framework_root + "/googletest",
    googletest_framework_root + "/googletest/include",
    googletest_framework_root + "/googlemock",
    googletest_framework_root + "/googlemock/include"
]

gtest_all_path = googletest_framework_root + "/googletest/src/gtest-all.cc"
gmock_all_path = googletest_framework_root + "/googlemock/src/gmock-all.cc"


def get_qt_install_prefix():
    qmake_output = subprocess.check_output(['qmake', '-query'])
    string_qmake = qmake_output.decode('utf-8')
    lines = string_qmake.split(os.linesep)
    
    qt_dir = None
    for line in lines:
        (key, value) = line.split(':', 1)
        if key == 'QT_INSTALL_PREFIX':
            qt_dir = value
            break

    return qt_dir


qt5_dir = ARGUMENTS.get('qt5_dir')

print("using qt5 dir ", qt5_dir)

if (qt5_dir is None):
    qt5_dir = get_qt_install_prefix()

if (qt5_dir is None):
    raise Exception(
        "Unable to locate the Qt directory, try using qt5_dir=THEDIR on the "
        + "command line."
    )
    

env = Environment(
    tools=['default', 'qt5'],
    QT5DIR=qt5_dir,
    CPPPATH=googletest_include_paths,
)
env['QT5_DEBUG'] = 1

maybe_term = os.environ.get('TERM')
if maybe_term:
    env['ENV']['TERM'] = maybe_term

env.EnableQt5Modules(['QtCore', 'QtWidgets', 'QtNetwork'])
env.Append(CCFLAGS=['-fPIC', '-std=c++11'])
env.Append(
    LIBS=[
        'pthread',
        'xlnt'
    ]
)

print("Prog emitter:", env['PROGEMITTER'])
print("Program is" + repr(env['BUILDERS']['Program'].emitter))


gtest = env.Object(gtest_all_path)
gmock = env.Object(gmock_all_path)
test_harness = env.Object("test_harness.cc")
test_utility = env.Object("test_utility.cc")
utility = env.Object("utility.cc")
http_getter = env.Object("http_getter.cc")
http_poster = env.Object("http_poster.cc")

env.Program(
    target='unit_tests',
    source=[
        "progress_reporter.cc",
        "settings.cc",
        "file_info_test.cc",
        "file_info.cc",
        "requests.cc",
        "figshare_gateway_test.cc",
        "figshare_gateway.cc",
        "license_test.cc",
        "article_type_mapper.cc",
        "token_store.cc",
        "article_type_mapper_test.cc",
        "article_mapper.cc",
        "path_extractor_test.cc",
        "path_extractor.cc",
        "category_test.cc",
        "category_mapper.cc",
        "stubs.cc",
        "md5_test.cc",
        "file_spec_generator.cc",
        "mapper_test.cc",
        "xlsx_test.cc",
        "part_preparer_test.cc",
        "part_preparer.cc",
        "file_part.cc",
        "upload_command.cc",
        "http_putter.cc",
        "upload_command_processor.cc",
        "upload_command_processor_test.cc",
        "fake_qt_core_application.cc",
        "http_getter.cc",
        "http_poster.cc",
        "http_getter_test.cc",
        "http_poster_test.cc",
        "xlsx.cc",
        "size_getter_integration_test.cc",
        "size_getter.cc",
        "requests_test.cc",
        "upload_container_info_test.cc",
        "upload_container_info.cc",
        "file_part_test.cc",
        "responses_test.cc",
        "responses.cc",
        "driver_test.cc",
        "driver.cc",
        "object_mother.cc",
        utility,
        test_harness, test_utility, gtest, gmock
    ]
)

env.Program(
    target='main',
    source=[
        "progress_reporter.cc",
        "settings.cc",
        "main.cc", "view.cc", "presenter.cc", "run_upload_task.cc",
        "driver_thread.cc",
        "file_info.cc",
        "requests.cc",
        "figshare_gateway.cc",
        "article_type_mapper.cc",
        "article_mapper.cc",
        "category_mapper.cc",
        "file_spec_generator.cc",
        "token_store.cc",
        "part_preparer.cc",
        "file_part.cc",
        "upload_command.cc",
        "http_putter.cc",
        "upload_command_processor.cc",
        "http_getter.cc",
        "http_poster.cc",
        "xlsx.cc",
        "size_getter.cc",
        "upload_container_info.cc",
        "responses.cc",
        "progress_reporter.cc",
        "driver.cc",
        "path_extractor.cc",
        "utility.cc"
    ]
)
