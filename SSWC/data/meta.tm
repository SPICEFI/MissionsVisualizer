KPL/MK

\begintext

	Blank line at the end of file is essential

\begindata

	PATH_VALUES  = (	'O:/NASA/Kernels' )

	PATH_SYMBOLS = (	'BASE' )

\begintext
	Leapseconds kernel
\begindata

	KERNELS_TO_LOAD = (		'$BASE/lsk/naif0011.tls.pc' )

\begintext
	Generic planetary ephemerides
\begindata

	KERNELS_TO_LOAD += (	'$BASE/spk/planets/de430.bsp' )

\begintext
	SPK kernel for Solar System bodies
\begindata

	KERNELS_TO_LOAD += (	'$BASE/spk/satellites/jup300.bsp',
							'$BASE/spk/satellites/jup310.bsp',
							'$BASE/spk/satellites/jup329.bsp',
							'$BASE/spk/satellites/mar097.bsp',
							'$BASE/spk/satellites/nep081.bsp',
							'$BASE/spk/satellites/nep086.bsp',
							'$BASE/spk/satellites/nep087.bsp',
							'$BASE/spk/satellites/nep088.bsp',
							'$BASE/spk/satellites/plu043.bsp',
							'$BASE/spk/satellites/sat319.bsp',
							'$BASE/spk/satellites/sat368.bsp',
							'$BASE/spk/satellites/sat375.bsp',
							'$BASE/spk/satellites/sat382.bsp',
							'$BASE/spk/satellites/sat382-rocks-merge.bsp',
							'$BASE/spk/satellites/ura091-rocks-merge.bsp',
							'$BASE/spk/satellites/ura111.bsp',
							'$BASE/spk/satellites/ura112.bsp' )

\begintext
	PCK kernel with shape and orientation
\begindata

	KERNELS_TO_LOAD += (	'$BASE/pck/pck00010.tpc' )

\begintext
	PCK kernel with high precision Earth orientation
\begindata

	KERNELS_TO_LOAD += (	'$BASE/pck/earth_720101_070426.bpc',
							'$BASE/pck/earth_070425_370426_predict.bpc'
							'$BASE/pck/earth_000101_151203_150911.bpc' )

\begintext
	PCK kernel with mass constants
\begindata

	KERNELS_TO_LOAD += (	'$BASE/pck/gm_de431.tpc' )

\begintext
	Override Earth default frame
\begindata

	KERNELS_TO_LOAD += (	'$BASE/fk/planets/earth_assoc_itrf93.tf' )

\begintext
	Test frame (not loaded)
begindata

	KERNELS_TO_LOAD += (	'data/frames.fk' )
