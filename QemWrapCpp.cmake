
MACRO(QEM_EXTRACT_OPTIONS _qem_files _qem_options)
  SET(${_qem_files})
  SET(${_qem_options})
  SET(_qem_DOING_OPTIONS FALSE)
  FOREACH(_currentArg ${ARGN})
	LIST(APPEND ${_qem_files} "${_currentArg}")
  ENDFOREACH(_currentArg)
ENDMACRO(QEM_EXTRACT_OPTIONS)

MACRO(qem_wrap_cpp outfiles destdir using_qt5)
	
	QEM_EXTRACT_OPTIONS(moc_files moc_options ${ARGN})

	foreach(it ${moc_files})

		get_filename_component(it ${it} ABSOLUTE)

		get_filename_component(it_base ${it} NAME)

        set(qemPreProcFile ${destdir}/${it_base}.qem)
		set(qtMocFile ${destdir}/moc_${it_base}.cxx)

        add_custom_command(OUTPUT ${qemPreProcFile}
										COMMAND ${QEMPreProcBin} "${it}" -o=${qemPreProcFile}
                                )
		add_custom_command(OUTPUT ${qtMocFile}
										COMMAND ${QT_MOC_EXECUTABLE} "${qemPreProcFile}" -f"${qemPreProcFile}" > "${qtMocFile}"
                                        DEPENDS ${qemPreProcFile}
                                )
		set(${outfiles} ${${outfiles}} ${qtMocFile})

	endforeach()

ENDMACRO(qem_wrap_cpp)
