@echo off

set OLD_DIR=%CD%

echo TRANSLATIONS = \
echo       %1/share/locale/mscore_da.ts \
echo       %1/share/locale/mscore_de.ts \
echo       %1/share/locale/mscore_ru.ts \
echo       %1/share/locale/mscore_it.ts \
echo       %1/share/locale/mscore_es.ts \
echo       %1/share/locale/mscore_fi.ts \
echo       %1/share/locale/mscore_fr.ts \
echo       %1/share/locale/mscore_nl.ts \
echo       %1/share/locale/mscore_cs.ts \
echo       %1/share/locale/mscore_gl.ts \
echo       %1/share/locale/mscore_tr.ts \
echo       %1/share/locale/mscore_hi_IN.ts \
echo       %1/share/locale/mscore_pt_BR.ts \
echo       %1/share/locale/mscore_pt.ts \
echo       %1/share/locale/mscore_sv.ts \
echo       %1/share/locale/mscore_no.ts \
echo       %1/share/locale/mscore_en_GB.ts \
echo       %1/share/locale/mscore_zh_Hant.ts




cd /d %1

echo FORMS = \
for /r %1 %%a in (*.ui) do echo     %%a \
echo.

echo SOURCES = \
for /r %1 %%a in (*.cpp) do echo     %%a \
echo.
echo.

cd /d %OLD_DIR%