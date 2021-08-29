map = Map("watsoniot-daemon")

section = map:section(NamedSection, "watsoniot_sct", "watson-daemon", "IoT Device")
flag = section:option(Flag, "enabled", "Enable", "Enable daemon")

orgId = section:option(Value, "orgId", "Organization ID")
typeId = section:option(Value, "typeId", "Type ID")
deviceId = section:option(Value, "deviceId", "Device ID")

auth_token = section:option(Value, "authToken", "Authentication Token")
auth_token.datatype = "credentials_validate"
auth_token.password = true

return map
