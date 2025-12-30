# MSVC build

Built passed in Visual Studio 2022.

Some resource files need to be generated mannually.

`resource.xrs`

Zip `..\src\gui\res\*.xrc` and rename it to the target.

`en\xrc.mo`
`en\HomeAccounts.mo`
`zh\xrc.mo`
`zh\HomeAccounts.mo`

These language files are generated from files in `..\src\gui\res\i18n\`. You can use [PoEdit](https://poedit.net/).

`lasyard_logo.png`

Copy from `..\src\gui\res\images\lasyard_logo.png`.

`app_icon.png`

Copy from `..\src\gui\res\macos.iconset\icon_512x512.png`.

Where to put these files:

- When run in the build tree, put them to where the solution file is, (i.e. this directory)
- When run in other place, put them to the same directory of the executable file
