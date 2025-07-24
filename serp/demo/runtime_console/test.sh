#!/usr/bin/expect

# To install expect
# sudo apt update
# sudo apt install expect

proc check_response {expected_response exit_on_error} {
    if {![info exists exit_on_error]} {
        set exit_on_error 0
    }
    expect {
        -re {.*\[OK\] Response: (\d+).*} {
            set response $expect_out(1,string)
            if {$response != $expected_response} {
                send_user "Error: Expected response '$expected_response', got: '$response'\n"
                if {$exit_on_error == 1} {
                    exit 1
                }
            }
        }
    }
}

proc send_and_check {command expected_response exit_on_error} {
    if {![info exists exit_on_error]} {
        set exit_on_error 0
    }

    expect "> "
    send "$command\r"
    if {$expected_response ne ""} {
        check_response $expected_response $exit_on_error
    }
}

set script_dir [file dirname [info script]]
set app_path "${script_dir}/../../build/demo/runtime_console/demo_runtime_console"

log_user 1

spawn $app_path

send_and_check "Foo()" "" 0
send_and_check "SayHello(world)" "" 0
send_and_check "GiveMeAnswer()" "42" 0
send_and_check "Add(2,3)" "5" 0
send_and_check "Add(123,321)" "444" 0

catch {expect eof} result

interact
