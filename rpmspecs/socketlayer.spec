Name:           socketlayer
Version:        2.1.2
Release:        1%{?dist}
Summary:        Socket abstraction library

License:        LGPLv2+        
Source0:        %{name}-%{version}.tar.gz

%description
Socket abstraction library for platform independent developement for Linux and Windows (using MinGW).

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%setup -q


%build
%configure --disable-static
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%doc
%{_libdir}/*.so.*

%files devel
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*

%doc
/usr/share/doc/socketlayer/*

%changelog
* Sat Aug 13 2016 Dirk Gottschalk
- Renamed functions
- Changed variables
- Defines socket_t as socket type for Linux and Windows

* Tue Dec 29 2015 Dirk Gottschalk
- First RPM release of libSocketlayer
