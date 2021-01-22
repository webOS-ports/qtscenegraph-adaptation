Name:       qtscenegraph-adaptation
Summary:    Scenegraph adaptation for Droid
Version:    git
Release:    1%{?dist}
License:    LGPLv2 with exception or GPLv3 or Qt Commercial
URL:        https://git.sailfishos.org/mer-core/qtscenegraph-adaptation
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  qt5-qtcore-devel
BuildRequires:  qt5-qtgui-devel
BuildRequires:  qt5-qtdeclarative-devel
BuildRequires:  qt5-qtdeclarative-qtquick-devel
BuildRequires:  qt5-qmake

%description
This package contains system specific changes for the
Qt Quick Scene Graph.

#### Build section

%prep
%setup -q -n %{name}-%{version}

%build
export QTDIR=/usr/share/qt5
%qmake5 -config "surfaceformat programbinary hybristexture" DEFINES+=EGL_NO_X11

%install
rm -rf %{buildroot}
%qmake5_install

#### Pre/Post section

%post
/sbin/ldconfig
%postun
/sbin/ldconfig




#### File section
%files
%defattr(-,root,root,-)
%license LICENSE.LGPL
%license LGPL_EXCEPTION.txt
%license LICENSE.GPL
%{_libdir}/qt5/plugins/scenegraph/libcustomcontext.so


#### No changelog section, separate $pkg.changelog contains the history
