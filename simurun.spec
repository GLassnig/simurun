%define name simurun
%define version 3.0
%define release lp152.1.1
%define packager Georg Lassnig

Name: %{name}
#Provides: %{name}
Summary: A small tool for the Simutrans Economic Simulation Game
Version: %{version}
Release:
License: GPL v3
Icon: %{name}.xpm
Group: Amusements/Games/Other
BuildRoot: %{_tmppath}/build-root-%{name}-%{version}-%{release}
Source0: %{name}-%{version}.tar.gz
Packager: %{packager}
Distribution: openSUSE
Url: http://www.e67-its/
Vendor: %{packager}

#BuildRequires:  cmake(KF5Config)
#BuildRequires:  cmake(KF5KIO) >= 5.16

BuildRequires:  gcc-c++
BuildRequires:  pkgconfig(Qt5Core) >= 5.12
BuildRequires:  libqt5-linguist-devel
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Network)

%description
Simutrans Starter is a tool to configure the Simutrans Economic Simulation Game
Each included pak set has a own config file
%description -l de
%prep
rm -rf %{buildroot}
mkdir %{buildroot}

%setup -q

%build
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_datadir}/%{name}
mkdir -p %{buildroot}%{_datadir}/doc/packages/%{name}
mkdir -p %{buildroot}%{_datadir}/locale/de/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/en/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/applications/
mkdir -p %{buildroot}%{_datadir}/doc/packages/%{name}/
mkdir -p %{buildroot}%{_datadir}/doc/packages/%{name}/images
mkdir -p %{buildroot}%{_datadir}/doc/packages/%{name}/tpl
mkdir -p %{buildroot}%{_datadir}/pixmaps/


lupdate-qt5 %{name}.pro
lrelease-qt5 %{name}.pro
qmake-qt5
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" \
make

%install
cp %{name} %{buildroot}%{_bindir}

cp translations/%{name}_de.qm %{buildroot}%{_datadir}/locale/de/LC_MESSAGES/
cp translations/%{name}_en.qm %{buildroot}%{_datadir}/locale/en/LC_MESSAGES/

cp -r tpl/ %{buildroot}%{_datadir}/doc/packages/%{name}/
cp images/%{name}.png %{buildroot}%{_datadir}/pixmaps/
cp images/simutrans.png %{buildroot}%{_datadir}/pixmaps/

cat > $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop << EOF
[Desktop Entry]
Encoding=UTF-8
Categories=Qt;KDE;Games;Simulation;
Comment=Simutrans Starter - A small tool for the Simutrans Economic Simulation Game
Comment[de]=Simutrans Starter - A small tool for the Simutrans Economic Simulation Game
Comment[en]=Simutrans Starter - Ein kleines Tool für die Simutrans-Wirtschafts-Simulation
Exec=%{name} -caption "%c" %u
GenericName=Simutrans Starter
GenericName[de]=Simutrans Starter
GenericName[en]=Simutrans Starter

Icon=%{name}
MimeType=
Name=Simutrans Starter
Name[de]=Simutrans Starter

StartupNotify=true
Terminal=false
Type=Application
Version=1.0
X-SuSE-translate=true
EOF

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/locale/de/LC_MESSAGES/%{name}_de.qm
%{_datadir}/locale/en/LC_MESSAGES/%{name}_en.qm
%{_datadir}/doc/packages/%{name}/
%{_datadir}/doc/packages/%{name}/tpl
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/pixmaps/simutrans.png
%doc AUTHORS COPYING ChangeLog NEWS README.md TODO %{name}.spec

%changelog
* Mon Jul 23 2012 Simutrans Starter - 2.2.1
- ADDED: Simple Drawing Tiles size.
- ADDED: Signals on left side if drive left = yes.
- ADDED: Nickname for network games.
- ADDED: New option for lowest possible income with speedbonus.
- ADDED: New dialog to lookup for program updates.
- UPDATED: English translation.

* Sat Feb 04 2012 Simutrans Starter - 2.2.0
- ADDED: Option for the height of tiles 'tile_height'
- ADDED: Option for Vehicle states 'show_vehicle_states'
- ADDED: Option for visualizing a schedule 'visualize_schedule'
- ADDED: Option for straight way calculation 'straight_way_without_control'
- UPDATED: english translation
- UPDATED: citylist_de_at.txt, more than 300 austrian cities and villages.
- FIXED: Wrong value saved for cost_maintain_transformer

* Sun Nov 13 2011 Simutrans Starter - 2.0.1
- ADDED: New options for network games
- ADDED: New options locality factor.
- ADDED: New options factory arrival periods.
- ADDED: New options toll runningcost percentage.
- ADDED: New options toll waycost percentage.
- ADDED: New options Pause server if no clients are connected.
- UPDATED: Dutch translation. NOT FINISHED!
- UPDATED: Italian translation. NOT FINISHED!
- UPDATED: Portuguese translation. NOT FINISHED!
- UPDATED: Spain translation.
- UPDATED: English translation.
- UPDATED: German translation.
- CHANGED: Path for simurun2.conf. New Path is '/home/your_home_dir/.simurun2/'
- CHANGED: The handling of the applications icons are now KDE compliant.

* Tue Jun 28 2011 Simutrans Starter - 2.0.1
- ADDED: New settings features for Simutrans 110.0.2

* Sat Jun 25 2011 Simutrans Starter - 2.0
- CHANGED: Mainwindow design.
- ADDED: A checkbox to start Simutrans as a game server.
- ADDED: The choice between local or network game.
- ADDED: An option in mainwindow to choose between local game and network game.
- ADDED: New starting money feature.
- ADDED: New Sroad settings feature.
- ADDED: I made a redevelopment of the configure and settings window. For ease of use and better overview.
- ADDED: New game settings for the next release of Simutrans. And there are many new settings features.

* Thu Aug 12 2010 Simutrans Starter - 1.99.10
- UPDATED: Language files.
- ADDED: Network settings for multyplayer games. Available with Simutrans 102.3.
- ADDED: Pakset Pak128 German (pak128.german).

* Fri May 14 2010 Simutrans Starter - 1.99.9
- NEW: Online Update-Check for the Application.
- NEW: Spanish translation.
- FIXED: Wrong path settings.
- CHANGED: Layout settings in the configurations window.
- UPDATED: Language files.

* Mon May 03 2010 Simutrans Starter - 1.99.8
- ADDED: MySQLoader now supports KDE-GUI-style settings for a Better integration to KDE.
- UPDATE: Translation files.
- ADDED: A .spec file to build rpm packages.
- FIXED: Various small bugfixes
- CHANGED: The classes and widgets to be changed step by step from Qt to KDE

* Tue Mar 23 2010 Simutrans Starter - 1.99.7
- UPDATED: Translation files.
- ADDED: An option to load automatic the last saved game on application start. If there are no saved games this option is disabled.
- CHANGED: Resolutions settings on main window replaced with music/sound settings.
- FIXED: Bug in the language selection and chioce of the default language on application start.
- REMOVED: Unused variables.

* Sun Dec 13 2009 Simutrans Starter - 1.99.6
- ADDED: Settings: Select favorite installed Browser to load Simutrans-Support-Pages in the web.
- ADDED: Some links to Simutrans pages in the web.
- ADDED: A button to open a Screenshot with Gimp.
- ADDED: Upgrade Game.It is now possible to select the installation path manually, too.
- ADDED: Upgrade PakSet.It is now possible to select the installation path manually, too.
- CHANGED: Only one instance can run at the same time.
- FIXED: Screenshot-Viewer crashed if last Screenshot was deleted.
- FIXED: Not all PakSets were listed after upgrade.
- FIXED: Not all PakSets were listed in Settings.

* Sat Aug 22 2009 Simutrans Starter - 1.99.5
- ADDED: openpak128 pak set.
- CHANGED: Some Functions for faster startup.
- ADDED: The Screenshot Viewer.
- ADDED: It is now possible to change the language in the settings window.
- ADDED: Dutch translation.
- CORRECTED: English Translation.

* Fri Jun 26 2009 Simutrans Starter - 1.99.4
- CHANGED: With the next release of Simutrans is a higher starting money possible. Currently ~21 Million Credits with the next release are ~1 Billion Credits possible.
- FIXED: The menu entry (simurun2.desktop) and the icon (simurun2.png) were not copied in their target directories.

* Mon Jun 22 2009 Simutrans Starter - 1.99.3
- ADDED: Max number of stations to the global simuconf.tab.
- ADDED: Max number of convoys (i.e. train or truck with trailer or single truck) to the global simuconf.tab.
- ADDED: Max number of lines (all line handles) to the global simuconf.tab.
- ADDED: pak set pak128.Britain.
- ADDED: More resolution settings on the mainwindow.
- FIXED: The pak set pages in settings are not shown correctly. If a pak set not installed.

* Wed Jun 10 2009 Simutrans Starter - 1.99.2
- COMPLETED: English translation.
- ADDED: River settings for pak128.

* Tue Jun 09 2009 Simutrans Starter - 1.99.1
- ADDED: Missed translations files.

* Mon Jun 08 2009 Simutrans Starter - 1.99.0
- Initial release of Simutrans Starter 2. Developed with C++ and the Qt4 UI-Framework.

* Sat Oct 04 2008 Simutrans Starter - 1.0.5
- ADDED: New configure features for SimuTrans 100.0 (Show random moving animals,
         animate the water and show random objects to break uniformity).
- ADDED: A new good looking icon for the application. ;o)
- CHANGED: The Settings window is now resizable.

* Sat Aug 23 2008 Simutrans Starter - 1.0.4
- FIXED: Removed static path in the makefile.in and makefile.am. Some files were not found when the 		 install-prefix was changed by configure.
- FIXED: Removed static path in the code.
- ADDED: The (incomplete) Simutrans Starter (de/en) documentation is available again.

* Sat Aug 16 2008 Simutrans Starter - 1.0.3
- FIXED: It is possible again to choose between austrian and german city names.
- FIXED: Bugs in the Makefile building.

* Sat Aug 02 2008 Simutrans Starter - 1.0.2
- FIXED: Trailing slash in pak file path

* Wed Jun 18 2008 Simutrans Starter - 1.0.1
- FIXED: cost_multiply_found_industry now saved correctly.
- FIXED: pak_file_path now saved correctly.

* Tue Apr 08 2008 Simutrans Starter - 1.0.0
- I think it is time to give it a leading 1. ;-)
- NEW: It is now possible to edit the music.tab in settings window
- CHANGED: Some code in install and update functions.
- ADDED: water_animation to Settings (maybe not work with Pak128).
- ADDED: random_grounds_probability to Settings (maybe not work with Pak128).
- ADDED: cost_transformer to Settings (maybe not work with Pak128).
- ADDED: cost_maintain_transformer to Settings (maybe not work with Pak128).
- ADDED: window_frame_active to Settings (maybe not work with Pak128).

* Mon Apr 07 2008 Simutrans Starter - 0.2.1
- NEW: Use now the Autotools for install

* Sun Feb 24 2008 Simutrans Starter - 0.2.0
- ADDED: an Install Wizzard for the Simutrans installation. Better look and feel! ;-)
- NEW: It is now possible to install Simutrans and update pak files with root access.
- FIXED: Screenshot Viewer show images now faster.
- A lot of code optimized.

* Wed Nov 07 2007 Simutrans Starter - 0.1.3
- Added a new screenshot viewer.

* Sun Nov 04 2007 Simutrans Starter - 0.1.2
- NEW: Save the simuconf.tab now in ~/.sumitrans by dafault and leave the file in the pak file folder untouched.
- NEW: Work with simtrans 99.15 binary.

* Sat Oct 06 2007 Simutrans Starter - 0.1.1
- Code cleaning.
- Added a modified version of the PictureDataBase 1.0 von Timothy Marshal-Nichols as Screenshot Viewer.

* Wed Aug 22 2007 Simutrans Starter - 0.1.0
- Code cleaning.
- ADDED: transformers cost and transformers maintain costs to the settings form availlable with simulinux 0.99.13. See http://forum.simutrans.com/index.php/topic,5974.0.html
- ADDED: a ComboBox for german and austrian user to choose german or austrian city names in the game. ;o)

* Fri Jul 06 2007 Simutrans Starter - 0.0.13
- Bugfix: The Application crashed if the screenshot folder not exist.

* Sat Jun 30 2007 Simutrans Starter - 0.0.12
- NEW: Licence is now GPL version 3
- Some changes in german and english translation.

* Thu Jun 28 2007 Simutrans Starter - 0.0.11
- FIXED: a bug in the backup procedures. Archives were empty.

* Tue Jun 26 2007 Simutrans Starter - 0.0.10
- ADDED: more game settings

* Fri Jun 15 2007 Simutrans Starter - 0.0.9
- NEW: The config files are not longer saved in "/home/user/.config". New directory is "/home/user/.simurun".
- ADDED: configuration tab for forestrules.tab
- ADDED: configuration tab for citytrules.tab

* Sun Jun 3 2007 Simutrans Starter - 0.0.8
- ADDED: a start class and changed the start options.
- Made a lot of code changes and bugfixes.
- ADDED: a documentation (Handbook de/en) incomplete but under heavy construction

* Sat Jun 2 2007 Simutrans Starter - 0.0.7
- Made some code changes in FrmStarter_main.class, FrmInstall.class, FrmPakInstall.class and FrmSettings.class.
- NEW: Application search now for an installed game in the home directory and ask for include it into Simutrans Starter.
- Gamepath is not longer saved in $HOME/simutrans/simutrans128.dir. You find it now in $HOME/.config/simupath.conf
- NEW: You can use Simutrans Starter now with other paks too. It use the pak_file_path variable in $HOME/.config/simurun.conf. All hardcoded pak128/ directions are removed.

* Sat May 26 2007 Simutrans Starter - 0.0.6
- FIXED: a bug in Fom Settings
- ADDED: a Makefile for install

* Tue May 22 2007 Simutrans Starter - 0.0.5
- ADDED: window to install additional Paks
- ADDED: a new logo
- ADDED: a new icon
- ADDED: english install file
- Some code changes and clean up
- FIXED: Bug with "pedes_and_car_info" to save in simuconf.tab (saved 2 times)
- FIXED: Bug with "random_pedestrians" to save in simuconf.tab (not saved)

* Sat May 12 2007 Simutrans Starter - 0.0.4
- FIXED: Bug in simuconf.tab creation (Compress saved games).

* Tue May 08 2007 Simutrans Starter - 0.0.3
- Another Bugfix release

* Mon Apr 16 2007 Simutrans Starter - 0.0.2
- Bugfix release

* Sun Apr 01 2007 Simutrans Starter - 0.0.1
- Initial release
