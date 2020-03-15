#ifndef _DOTNETNATIVE_SYSTEM_GLOBALIZATION_CULTUREDATA_H_
#define _DOTNETNATIVE_SYSTEM_GLOBALIZATION_CULTUREDATA_H_

#include "../String.h"
#include "../Array.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Globalization
        {
            /// <summary>
            /// List of culture data
            /// Note the we cache overrides.
            /// Note that localized names (resource names) aren't available from here.
            /// </summary>
            /// <remarks>
            /// Our names are a tad confusing.
            ///
            /// sWindowsName -- The name that windows thinks this culture is, ie:
            ///                            en-US if you pass in en-US
            ///                            de-DE_phoneb if you pass in de-DE_phoneb
            ///                            fj-FJ if you pass in fj (neutral, on a pre-Windows 7 machine)
            ///                            fj if you pass in fj (neutral, post-Windows 7 machine)
            ///
            /// sRealName -- The name you used to construct the culture, in pretty form
            ///                       en-US if you pass in EN-us
            ///                       en if you pass in en
            ///                       de-DE_phoneb if you pass in de-DE_phoneb
            ///
            /// sSpecificCulture -- The specific culture for this culture
            ///                             en-US for en-US
            ///                             en-US for en
            ///                             de-DE_phoneb for alt sort
            ///                             fj-FJ for fj (neutral)
            ///
            /// sName -- The IETF name of this culture (ie: no sort info, could be neutral)
            ///                en-US if you pass in en-US
            ///                en if you pass in en
            ///                de-DE if you pass in de-DE_phoneb
            /// </remarks>
            class CultureData
            {
            private:
                static constexpr int LocaleNameMaxLength = 85;
                static constexpr int undef = -1;

                // Override flag
                String _sRealName; // Name you passed in (ie: en-US, en, or de-DE_phoneb). Initialized by helper called during initialization.
                String _sWindowsName; // Name OS thinks the object is (ie: de-DE_phoneb, or en-US (even if en was passed in))

                // Identity
                String _sName; // locale name (ie: en-us, NO sort info, but could be neutral)
                String _sParent; // Parent name (which may be a custom locale/culture)
                String _sLocalizedDisplayName; // Localized pretty name for this locale
                String _sEnglishDisplayName; // English pretty name for this locale
                String _sNativeDisplayName; // Native pretty name for this locale
                String _sSpecificCulture; // The culture name to be used in CultureInfo.CreateSpecificCulture(), en-US form if neutral, sort name if sort

                // Language
                String _sISO639Language; // ISO 639 Language Name
                String _sISO639Language2; // ISO 639 Language Name
                String _sLocalizedLanguage; // Localized name for this language
                String _sEnglishLanguage; // English name for this language
                String _sNativeLanguage; // Native name of this language
                String _sAbbrevLang; // abbreviated language name (Windows Language Name) ex: ENU
                String _sConsoleFallbackName; // The culture name for the console fallback UI culture
                int    _iInputLanguageHandle = undef;// input language handle

                // Region
                String _sRegionName; // (RegionInfo)
                String _sLocalizedCountry; // localized country name
                String _sEnglishCountry; // english country name (RegionInfo)
                String _sNativeCountry; // native country name
                String _sISO3166CountryName; // ISO 3166 (RegionInfo), ie: US
                String _sISO3166CountryName2; // 3 char ISO 3166 country name 2 2(RegionInfo) ex: USA (ISO)
                int    _iGeoId = undef; // GeoId

                // Numbers
                String _sPositiveSign; // (user can override) positive sign
                String _sNegativeSign; // (user can override) negative sign
                // (nfi populates these 5, don't have to be = undef)
                int _iDigits; // (user can override) number of fractional digits
                int _iNegativeNumber; // (user can override) negative number format
                Array<int> _waGrouping; // (user can override) grouping of digits
                String _sDecimalSeparator; // (user can override) decimal separator
                String _sThousandSeparator; // (user can override) thousands separator
                String _sNaN; // Not a Number
                String _sPositiveInfinity; // + Infinity
                String _sNegativeInfinity; // - Infinity

                // Percent
                int _iNegativePercent = undef; // Negative Percent (0-3)
                int _iPositivePercent = undef; // Positive Percent (0-11)
                String _sPercent; // Percent (%) symbol
                String _sPerMille; // PerMille symbol

                // Currency
                String _sCurrency; // (user can override) local monetary symbol
                String _sIntlMonetarySymbol; // international monetary symbol (RegionInfo)
                String _sEnglishCurrency; // English name for this currency
                String _sNativeCurrency; // Native name for this currency
                // (nfi populates these 4, don't have to be = undef)
                int _iCurrencyDigits; // (user can override) # local monetary fractional digits
                int _iCurrency; // (user can override) positive currency format
                int _iNegativeCurrency; // (user can override) negative currency format
                Array<int> _waMonetaryGrouping; // (user can override) monetary grouping of digits
                String _sMonetaryDecimal; // (user can override) monetary decimal separator
                String _sMonetaryThousand; // (user can override) monetary thousands separator

                // Misc
                int _iMeasure = undef; // (user can override) system of measurement 0=metric, 1=US (RegionInfo)
                String _sListSeparator; // (user can override) list separator

                // Time
                String _sAM1159; // (user can override) AM designator
                String _sPM2359; // (user can override) PM designator
                String _sTimeSeparator;
                Array<String> _saLongTimes; // (user can override) time format
                Array<String> _saShortTimes; // short time format
                Array<String> _saDurationFormats; // time duration format

                // Calendar specific data
                int _iFirstDayOfWeek = undef; // (user can override) first day of week (gregorian really)
                int _iFirstWeekOfYear = undef; // (user can override) first week of year (gregorian really)
                Array<CalendarId> _waCalendars; // all available calendar type(s).  The first one is the default calendar

                // Store for specific data about each calendar
                CalendarData ? [] ? _calendars; // Store for specific calendar data

                // Text information
                int _iReadingLayout = undef; // Reading layout data
                // 0 - Left to right (eg en-US)
                // 1 - Right to left (eg arabic locales)
                // 2 - Vertical top to bottom with columns to the left and also left to right (ja-JP locales)
                // 3 - Vertical top to bottom with columns proceeding to the right

                // CoreCLR depends on this even though its not exposed publicly.

                int _iDefaultAnsiCodePage = undef; // default ansi code page ID (ACP)
                int _iDefaultOemCodePage = undef; // default oem code page ID (OCP or OEM)
                int _iDefaultMacCodePage = undef; // default macintosh code page
                int _iDefaultEbcdicCodePage = undef; // default EBCDIC code page

                int _iLanguage; // locale ID (0409) - NO sort information
                bool _bUseOverrides; // use user overrides?
                bool _bNeutral; // Flags for the culture (ie: neutral or not right now)
            };
        }
    }
}

#endif