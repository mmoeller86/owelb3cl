/* UTF-8 encoding, ¥ */

#define PRODUCT_NAME    banner1t( "Zoom" )

// NB the maximum allowable string length is 128 bytes

pick(   STR_APP_NAME,
        PRODUCT_NAME,
        PRODUCT_NAME )
pick(   STR_ABOUT_VER,
        "",     //NOT USED
        "" )
pick(   STR_ABOUT_TITLE,
        "About " PRODUCT_NAME,
        "About " PRODUCT_NAME )
pick(   STR_UNABLE_TO_AUTOREFRESH,
        "Unable to do autorefresh.  No system timer available",
        "自動更新が行えません.  使用可能なシステム タイマーがありません" )
pick(   STR_INTERV_NOT_NUMERIC,
        "The Interval field must contain a numeric value",
        "間隔は数値で指定しなければなりません" )
pick(   STR_ABOUT_NAME,
        PRODUCT_NAME " for Windows",
        PRODUCT_NAME " for Windows" )
pick(   STR_ABOUT_VERSION,
        banner1v( _ZOOM_VERSION_ ),
        banner1v( _ZOOM_VERSION_ ) )
pick(   STR_INTERV_IS_ZERO,
        "The refresh interval must be greater than zero.",
        "更新間隔は 0 より大きい値です." )
