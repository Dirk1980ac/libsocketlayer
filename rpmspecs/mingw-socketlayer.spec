%{?mingw_package_header}

Name:           mingw-socketlayer
Version:        2.1.2
Release:        1%{?dist}
Summary:        MinGW socket abstraction layer

License:        LGPLv2+
Source0:        socketlayer-%{version}.tar.gz

BuildArch:      noarch

BuildRequires:  mingw32-filesystem >= 95
BuildRequires:  mingw64-filesystem >= 95
BuildRequires:  mingw32-gcc
BuildRequires:  mingw64-gcc
BuildRequires:  mingw32-binutils
BuildRequires:  mingw64-binutils
BuildRequires:  automake autoconf libtool

%description
Socket abstraction library for platform independent developement for Linux and Windows (using MinGW).

# Mingw32
%package -n mingw32-socketlayer
Summary:        MinGW socket abstraction layer

%description -n mingw32-socketlayer
Socket abstraction library for platform independent developement for Linux and Windows (using MinGW).

%package -n mingw32-socketlayer-static
Summary:        Static version of the MinGW socket abstraction layer
Requires:       mingw32-socketlayer = %{version}-%{release}


%description -n mingw32-socketlayer-static
Static version of the MinGW socket abstraction layer.


# Mingw64
%package -n mingw64-socketlayer
Summary:        MinGW socket abstraction layer

%description -n mingw64-socketlayer
Socket abstraction library for platform independent developement for Linux and Windows (using MinGW).

%package -n mingw64-socketlayer-static
Summary:        Static version of the MinGW socket abstraction layer
Requires:       mingw32-socketlayer = %{version}-%{release}


%description -n mingw64-socketlayer-static
Static version of the MinGW socket abstraction layer.


%?mingw_debug_package


%prep
%setup -q -n socketlayer-%{version}


%build
./autogen.sh
make distclean
%mingw_configure
%mingw_make %{?_smp_mflags} V=1


%install
%mingw_make_install DESTDIR=$RPM_BUILD_ROOT

find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'
find $RPM_BUILD_ROOT -name cpio.5 -exec rm -f {} ';'
find $RPM_BUILD_ROOT -name mtree.5 -exec rm -f {} ';'
find $RPM_BUILD_ROOT -name tar.5 -exec rm -f {} ';'


# Win32
%files -n mingw32-socketlayer
%license COPYING
%doc %{mingw32_docdir}/*
%{mingw32_bindir}/libsocketlayer-0.dll
%{mingw32_includedir}/socketlayer.h
%{mingw32_libdir}/libsocketlayer.dll.a
%{mingw32_libdir}/pkgconfig/socketlayer.pc

%files -n mingw32-socketlayer-static
%{mingw32_libdir}/libsocketlayer.a

# Win64
%files -n mingw64-socketlayer
%license COPYING
%doc %{mingw64_docdir}/*

%{mingw64_bindir}/libsocketlayer-0.dll
%{mingw64_includedir}/socketlayer.h
%{mingw64_libdir}/libsocketlayer.dll.a
%{mingw64_libdir}/pkgconfig/socketlayer.pc

%files -n mingw64-socketlayer-static
%{mingw64_libdir}/libsocketlayer.a

%changelog
* Sat Aug 13 2016 Dirk Gottschalk
- Renamed functions
- Changed variables
- Defines socket_t as socket type for Linux and Windows

* Tue Dec 29 2015 Dirk Gottschalk
- First RPM release of libSocketlayer
