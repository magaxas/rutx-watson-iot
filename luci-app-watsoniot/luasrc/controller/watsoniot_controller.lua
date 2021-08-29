module("luci.controller.watsoniot_controller", package.seeall)

function index()
    entry(
        {"admin", "services", "watsoniot"},
        cbi("watsoniot_model"),
        "IBM IoT platform"
    )
end
