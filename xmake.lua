set_encodings("utf-8") -- msvc: /utf-8
set_encodings("source:utf-8", "target:utf-8")

target("clib-util")
    set_kind("headeronly")
    add_headerfiles("include/**.h", "include/**.hpp")
    add_includedirs("include", {public = true})
    