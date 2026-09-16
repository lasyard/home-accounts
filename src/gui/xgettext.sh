#!/usr/bin/env bash

DIR="res/i18n"

if [ ! -d "${DIR}" ]; then
    mkdir -p "${DIR}"
fi

APP_POT_NAME=HomeAccounts
XRC_POT_NAME=xrc

OPTIONS="--copyright-holder=Lasy
--package-name=${APP_POT_NAME}
--package-version=1.0.0
--msgid-bugs-address=lasyard@yeah.net"

XGETTEXT_ARGS="-C -k_ \
    -kwxPLURAL:1,2 \
    -kwxGETTEXT_IN_CONTEXT:1c,2 \
    -kwxGETTEXT_IN_CONTEXT_PLURAL:1c,2,3 \
    -kwxTRANSLATE \
    -kwxTRANSLATE_IN_CONTEXT:1c,2 \
    -kwxGetTranslation \
    "

xgettext ${XGETTEXT_ARGS} -f <(find . -name '*.cpp' -o -name '*.h') -p "${DIR}" -o ${APP_POT_NAME}.pot ${OPTIONS}

wxrc -g res/*.xrc | xgettext ${XGETTEXT_ARGS} -p "${DIR}" -o ${XRC_POT_NAME}.pot - ${OPTIONS}

for domain in "${APP_POT_NAME}" "${XRC_POT_NAME}"; do
    for locale in "en" "zh"; do
        file="${DIR}/${domain}_${locale}.po"
        if [ -f "${file}" ]; then
            msgmerge -U "${file}" "${DIR}/${domain}.pot"
        else
            msginit -i "${DIR}/${domain}.pot" -o "${file}" --locale ${locale} --no-translator --no-wrap
        fi
    done
done
