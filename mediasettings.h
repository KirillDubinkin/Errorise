/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _MEDIASETTINGS_H_
#define _MEDIASETTINGS_H_


/* Settings the user has set for this file, and that we need to */
/* restore the video after a restart */

#include <QString>
#include <QSize>
#include "config.h"
//#include "audioequalizerlist.h"

class QSettings;

class MediaSettings {

public:
	enum Denoise { NoDenoise = 0, DenoiseNormal = 1, DenoiseSoft = 2 };

        enum AudioChannels { ChDefault = 0, ChStereo = 2, ChSurround = 4,
                         ChFull51 = 6 };
	enum StereoMode { Stereo = 0, Left = 1, Right = 2 };

	enum IDs { NoneSelected = -1000, SubNone = 90000 };

	MediaSettings();
	virtual ~MediaSettings();

	virtual void reset();

        //double current_sec;
        int current_sec;

#if PROGRAM_SWITCH
	int current_program_id;
#endif

	int current_audio_id;

	int volume;
	bool mute;

//	AudioEqualizerList audio_equalizer;


	bool karaoke_filter;
	bool extrastereo_filter;
	bool volnorm_filter;

	int audio_use_channels;
	int stereo_mode;

	bool loop; //!< Loop. If true repeat the file

	double starting_time; // Some videos don't start at 0

	//! The codec of the video is ffh264 and it's high definition

	// Advanced settings
	QString forced_demuxer;
	QString forced_audio_codec;

	// A copy of the original values, so we can restore them.
	QString original_demuxer;
        QString original_audio_codec;

	// Options to mplayer (for this file only)
	QString mplayer_additional_options;
	QString mplayer_additional_audio_filters;

	void list();

#ifndef NO_USE_INI_FILES
	void save(QSettings * set);
	void load(QSettings * set);
#endif
};

#endif
