# figshare-uploader (aka upgraded-fortnight)

For docs on how to build the thing, read [doc/BUILD.md](doc/BUILD.md).

_figshare-uploader_ is a bulk upload tool for the research archiving & storage
provider, Figshare.

It will accept spreadsheets in `.xlsx` format as input, upload the files listed
therein, with their associated metadata.

It is a GUI program that works on Windows, Mac, and Linux.  It supports uploading
files of unlimited size.

A personal token is required to access your Figshare account.  Please read the
[instructions](doc/creating_personal_token.md) for getting one.

## Guide

The program works simply.  You prepare your uploads in Excel format, following a
certain schema.  The settings allow flexible mapping of Excel fields.  If you
want a demo to get started, you can follow the [example upload
sheet](resources/basic_schema_upload.xlsx).  Using this format, you should load
the mapping configuration `basic_schema.json`.  You can reconfigure it for your
needs.

## Install

Currently there's a .dmg file for Mac, a zip for windows (no installer), and
Linux users have to build it yourselves.

## Limitations

Currently the tool does not operate as a 'synchronization': that is, if you
already have the articles present and uploaded, it will attempt to upload them
again.

It does not have support for pause and resuming, of the kind that the API would
allow, and dealing with intermittent connections.  That kind of support would
be useful when dealing with upload of very large files.

It only supports `.xlsx`.

## Support

This work was supported by the [Sussex Humanities Lab](http://www.sussex.ac.uk/shl).

## License

Apache 2.0, see the `LICENSE` file.
