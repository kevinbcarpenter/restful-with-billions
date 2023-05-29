include(CheckCXXCompilerFlag)
function(enable_cxx_flag_if_supported target visibility flag)
    CHECK_CXX_COMPILER_FLAG(${flag} SupportCurrentFlag)
    if(${SupportCurrentFlag})
        target_compile_options(${target}
            ${visibility} ${flag}
        )
    endif()
endfunction( )