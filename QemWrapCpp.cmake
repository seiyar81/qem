
if(NOT qem_wrap_cpp_loaded)

set(qem_wrap_cpp_loaded			true)

MACRO(qem_wrap_cpp outfiles destdir)

	# get include dirs
	QT4_GET_MOC_FLAGS(moc_flags)
	QT4_EXTRACT_OPTIONS(moc_files moc_options ${ARGN})

	foreach(it ${moc_files})

		get_filename_component(it ${it} ABSOLUTE)
		QT4_MAKE_OUTPUT_FILE(${it} moc_ cxx outfile)

		get_filename_component(it_base ${it} NAME)

        set(qemPreProcFile ${destdir}/${it_base}.qem)
		set(qtMocFile ${destdir}/moc_${it_base}.cxx)

                add_custom_command(OUTPUT ${qemPreProcFile}
                                        COMMAND ${QEMPreProcBin} "${it}" -o${qemPreProcFile}
                                )
		add_custom_command(OUTPUT ${qtMocFile}
                                        COMMAND moc "${qemPreProcFile}" -f"${qemPreProcFile}" > "${qtMocFile}"
                                        DEPENDS ${qemPreProcFile}
                                )
		set(${outfiles} ${${outfiles}} ${qtMocFile})

	endforeach()

ENDMACRO(qem_wrap_cpp)

endif(NOT qem_wrap_cpp_loaded)
