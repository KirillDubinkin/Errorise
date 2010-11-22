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

#include "mediasettings.h"
#include "preferences.h"
#include "global.h"
#include <QSettings>

using namespace Global;

MediaSettings::MediaSettings() {
	reset();
}

MediaSettings::~MediaSettings() {
}

void MediaSettings::reset() {
	current_sec = 0;

#if PROGRAM_SWITCH
	current_program_id = NoneSelected;
#endif
        current_id = NoneSelected;

	volume = pref->initial_volume;
        mute = false;
//	audio_equalizer = pref->initial_audio_equalizer;

        karaoke_filter = false;
	extrastereo_filter = false;
	volnorm_filter = pref->initial_volnorm;

	audio_use_channels = pref->initial_audio_channels; //ChDefault; // (0)
	stereo_mode = pref->initial_stereo_mode; //Stereo; // (0)

	starting_time = -1; // Not set yet.

	loop = false;

	forced_demuxer="";
        forced_audio_codec="";

	original_demuxer="";
        original_audio_codec="";

	mplayer_additional_options="";
	mplayer_additional_audio_filters="";

}


void MediaSettings::list() {
	qDebug("MediaSettings::list");

        qDebug("  current_sec: %d", current_sec);
#if PROGRAM_SWITCH
	qDebug("  current_program_id: %d", current_program_id);
#endif
        qDebug("  current_audio_id: %d", current_id);

	qDebug("  volume: %d", volume);
	qDebug("  mute: %d", mute);

	qDebug("  karaoke_filter: %d", karaoke_filter);
	qDebug("  extrastereo_filter: %d", extrastereo_filter);
	qDebug("  volnorm_filter: %d", volnorm_filter);

	qDebug("  audio_use_channels: %d", audio_use_channels);
	qDebug("  stereo_mode: %d", stereo_mode);


	qDebug("  forced_demuxer: '%s'", forced_demuxer.toUtf8().data());
        qDebug("  forced_audio_codec: '%s'", forced_audio_codec.toUtf8().data());

	qDebug("  original_demuxer: '%s'", original_demuxer.toUtf8().data());
        qDebug("  original_audio_codec: '%s'", original_audio_codec.toUtf8().data());

	qDebug("  mplayer_additional_options: '%s'", mplayer_additional_options.toUtf8().data());
	qDebug("  mplayer_additional_audio_filters: '%s'", mplayer_additional_audio_filters.toUtf8().data());

	qDebug("  starting_time: %f", starting_time);
}

#ifndef NO_USE_INI_FILES
void MediaSettings::save(QSettings * set) {
	qDebug("MediaSettings::save");

	//QSettings * set = settings;

        /*set->beginGroup( "mediasettings" );*/

        set->setValue( "current_sec", current_sec );
#if PROGRAM_SWITCH
	set->setValue( "current_program_id", current_program_id );
#endif
        set->setValue( "current_audio_id", current_id );

        set->setValue( "volume", volume );
        set->setValue( "mute", mute );
//        set->setValue("audio_equalizer", audio_equalizer );

	set->setValue( "karaoke_filter", karaoke_filter);
	set->setValue( "extrastereo_filter", extrastereo_filter);
	set->setValue( "volnorm_filter", volnorm_filter);

	set->setValue( "audio_use_channels", audio_use_channels);
	set->setValue( "stereo_mode", stereo_mode);

        set->setValue( "forced_demuxer", forced_demuxer);
	set->setValue( "forced_audio_codec", forced_audio_codec);

	set->setValue( "original_demuxer", original_demuxer);
        set->setValue( "original_audio_codec", original_audio_codec);

	set->setValue( "mplayer_additional_options", mplayer_additional_options);
	set->setValue( "mplayer_additional_audio_filters", mplayer_additional_audio_filters);

        set->setValue( "starting_time", starting_time );

	/*set->endGroup();*/
}

void MediaSettings::load(QSettings * set) {
	qDebug("MediaSettings::load");

	//QSettings * set = settings;

	/*set->beginGroup( "mediasettings" );*/

	current_sec = set->value( "current_sec", current_sec).toDouble();
#if PROGRAM_SWITCH
	current_program_id = set->value( "current_program_id", current_program_id ).toInt();
#endif
        current_id = set->value( "current_audio_id", current_id ).toInt();

        volume = set->value( "volume", volume ).toInt();
	mute = set->value( "mute", mute ).toBool();
//	audio_equalizer = set->value("audio_equalizer", audio_equalizer ).toList();

	karaoke_filter = set->value( "karaoke_filter", karaoke_filter).toBool();
	extrastereo_filter = set->value( "extrastereo_filter", extrastereo_filter).toBool();
	volnorm_filter = set->value( "volnorm_filter", volnorm_filter).toBool();

	audio_use_channels = set->value( "audio_use_channels", audio_use_channels).toInt();
	stereo_mode = set->value( "stereo_mode", stereo_mode).toInt();

	forced_demuxer = set->value( "forced_demuxer", forced_demuxer).toString();
	forced_audio_codec = set->value( "forced_audio_codec", forced_audio_codec).toString();

	original_demuxer = set->value( "original_demuxer", original_demuxer).toString();
        original_audio_codec = set->value( "original_audio_codec", original_audio_codec).toString();

	mplayer_additional_options = set->value( "mplayer_additional_options", mplayer_additional_options).toString();
	mplayer_additional_audio_filters = set->value( "mplayer_additional_audio_filters", mplayer_additional_audio_filters).toString();

	starting_time = set->value( "starting_time", starting_time ).toDouble();


       /*set->endGroup();*/

	// ChDefault not used anymore
	if (audio_use_channels == ChDefault) audio_use_channels = ChStereo;
}

#endif // NO_USE_INI_FILES
