#!/usr/bin/env sh

DIR="res/i18n"

if [ ! -d "${DIR}" ]; then
    mkdir -p "${DIR}"
fi

POT="${DIR}/messages.pot"

xgettext --c++ --no-location -k_ -f xgettext.list -o "${POT}" \
    --copyright-holder=Lasy \
    --package-name=home-accounts \
    --package-version=1.0.0 \
    --msgid-bugs-address=lasyard@yeah.net

for locale in "en" "zh"; do
    file="${DIR}/${locale}.po"
    if [ -f "${file}" ]; then
        msgmerge -U "${file}" "${POT}"
    else
        msginit -i "${POT}" -o "${file}" --locale ${locale} --no-translator
    fi
done
