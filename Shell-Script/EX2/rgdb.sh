#!/bin/sh
DBG_PRN_LOG=1

_msg()
{
    echo $'\n'"$1"$'\n'
}

function GtkLogPrint()
{
    if [ ${DBG_PRN_LOG} -ne 0 ]
    then
        echo -e "[GTK_DBG] $@"
    fi
}

MSG_USAGE=$(cat <<-END
  Usage:
    $0 slogz [syslogSize] - reset syslog size and restart syslog service.
END
 )

function slogz_handler()
{
    if [ $slogSize -gt 0 ];
    then
        uci set rmdbg.syslog.syslogSize=$slogSize
        uci commit
        /etc/init.d/log restart
    fi
}

switch_cli_table=(
    [0]="rmon_handler"
    [1]="mac_handler"
)

function rmon_handler()
{
    GtkLogPrint "rmon_handler"
}

function mac_handler()
{
    GtkLogPrint "mac_handler"
}

function sc_handler()
{
    #GtkLogPrint "Total arguments passed: $#"
    #GtkLogPrint "Total arguments : $@"
    handler="${switch_cli_table[$1]}"
    if [ -n "$handler" ] && [ "$(type -t "$handler")" = "function" ]; then
        "$handler" "${@:2}"
        break
    else
        echo "Handler not found for '$1'"
        exit 1
    fi
}

## Main function
GtkLogPrint "Welcome GTK REMOTE DEBUG CONTROL SCRIPT"

while [ $# -gt 0 ]
do
    case "$1" in
        -d)
            DBG_PRN_LOG=1
            ;;
        "slogz")
            slogSize=$2
            slogz_handler
            shift
            ;;
        "sc")
            shift
            sc_handler "$@"
            break
            ;;
        *)
            echo -e ""
            echo -e "ERROR: contain incorrect parameters"
            _msg "$MSG_USAGE"
            ;;
    esac
    shift
done

