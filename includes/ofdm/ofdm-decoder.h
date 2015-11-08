#
/*
 *
 *    Copyright (C) 2013
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Programming
 *
 *    This file is part of the SDR-J (JSDR).
 *    SDR-J is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    SDR-J is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with SDR-J; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef	__OFDM_DECODER
#define	__OFDM_DECODER

#define	MULTI_CORE
#include	"dab-constants.h"
#include	<QThread>
#ifdef	MULTI_CORE
#include	<QWaitCondition>
#include	<QMutex>
#else
#include	<QObject>
#endif
#include	"fft.h"
#include	"mapper.h"
#include	"phasetable.h"
#include	<stdint.h>

class	RadioInterface;
class	ficHandler;
class	mscHandler;

class	ofdmDecoder: public QThread {
Q_OBJECT
public:
		ofdmDecoder		(DabParams *,
	                                 RadioInterface *,
	                                 ficHandler	*,
	                                 mscHandler	*);
		~ofdmDecoder		(void);
	void	initTables		(void);
	void	processBlock_0		(DSPCOMPLEX *);
	void	decodeFICblock		(DSPCOMPLEX *, int32_t n);
	void	decodeMscblock		(DSPCOMPLEX *, int32_t n);
	int16_t	get_snr			(DSPCOMPLEX *);
	int16_t	coarseCorrector		(void);
private:
	DabParams	*params;
	RadioInterface	*myRadioInterface;
	ficHandler	*my_ficHandler;
	mscHandler	*my_mscHandler;
#ifdef	MULTI_CORE
	void		run		(void);
	bool		running;
	DSPCOMPLEX	**command;
	int16_t		amount;
	int16_t		currentBlock;
	void		processBlock_0		(void);
	void		decodeFICblock		(int32_t n);
	void		decodeMscblock		(int32_t n);
	QWaitCondition	commandHandler;
	QMutex		helper;
#endif
	int32_t		T_s;
	int32_t		T_u;
	int32_t		T_g;
	int32_t		carriers;
	int16_t		getMiddle	(DSPCOMPLEX *);
	DSPCOMPLEX	*phaseReference;
	common_fft	*fft_handler;
	DSPCOMPLEX	*fft_buffer;
	permVector	*myMapper;
	phaseTable	*phasetable;
	int32_t		blockIndex;
	int16_t		*ibits;
	int16_t		coarseOffset;
	int16_t		snrCount;
	int16_t		snr;
signals:
	void		show_snr	(int);
};

#endif

