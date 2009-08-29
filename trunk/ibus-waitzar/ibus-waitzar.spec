Name:       ibus-waitzar
Version:    1.0.0
Release:    1%{?dist}
Summary:    (later).... read requirements
License:    GPLv3+
Group:      System Environment/Libraries
URL:        http://code.google.com/p/scim-waitzar/
Source:    http://scim-waitzar.googlecode.com/files/%{name}-%{version}.tar.gz

BuildRoot:  %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildArch:  noarch

Requires:   ibus

%description
(later).... read requirements

%prep
%setup -q

%build
%configure --disable-static
# make -C po update-gmo
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=${RPM_BUILD_ROOT} NO_INDEX=true install

%find_lang %{name}

%clean
rm -rf $RPM_BUILD_ROOT

%post

%files -f %{name}.lang
%defattr(-,root,root,-)
%doc AUTHORS COPYING README
%{_datadir}/ibus-waitzar
%{_datadir}/ibus/component/*
%{_libexecdir}/ibus-engine-waitzar
%{_libexecdir}/ibus-setup-waitzar

%changelog
(LATER), read format

