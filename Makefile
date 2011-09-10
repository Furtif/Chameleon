#	Makefile for kernel booter
SRCROOT = $(CURDIR)
OBJROOT = $(SRCROOT)/obj
SYMROOT = $(SRCROOT)/sym
DSTROOT = $(SRCROOT)/dst
DOCROOT = $(SRCROOT)/doc
IMGSKELROOT = $(SRCROOT)/imgskel
CDBOOT = ${IMGROOT}/usr/standalone/i386/cdboot


include Make.rules


THEME = default

VERSION = `cat ${SRCROOT}/version`
REVISION = `cat ${SRCROOT}/revision`
PRODUCT = Chameleon-$(VERSION)-r$(REVISION)
CDLABEL = ${PRODUCT}
ISOIMAGE = ${SYMROOT}/${CDLABEL}.iso
DISTFILE = ${SYMROOT}/${PRODUCT}

IMGROOT = $(SRCROOT)/sym/${PRODUCT}
DISTROOT= ./${PRODUCT}


EXCLUDE = --exclude=.svn --exclude=.DS_Store --exclude=sym --exclude=obj \
		--exclude=package --exclude=archive --exclude=User_Guide_src --exclude=*.sh

#RC_CFLAGS = i386
ARCHLESS_RC_CFLAGS=`echo $(RC_CFLAGS) | sed 's/-arch [a-z0-9]*//g'`

GENERIC_SUBDIRS =
SUBDIRS = $(GENERIC_SUBDIRS) i386
DIST_SUBDIRS = $(SUBDIRS)

$(SRCROOT)/revision:
	@svnversion -n | tr -d [:alpha:] > $(SRCROOT)/revision

#
# Currently builds for i386
#
config rebuild_config:
	@make -C $(SRCROOT)/i386/config $@

all: $(SYMROOT) $(OBJROOT) $(CONFIG_HEADERS) $(HEADER_VERSION) $(SRCROOT)/revision
	@$(MAKE) all-recursive

dist image: all
	@# To force the read of auto.conf (generated by 'all' target)
	@make $@-local

dist-local image-local:
	@echo "================= Distrib ================="
	@echo "\t[RM] ${IMGROOT}"
	@rm -rf ${IMGROOT}	
	@echo "\t[MKDIR] ${IMGROOT}/usr/standalone/i386"			  	  
	@mkdir -p ${IMGROOT}/usr/standalone/i386
	@echo "\t[MKDIR] ${IMGROOT}/Extra/modules"
	@mkdir -p ${IMGROOT}/Extra/modules				
	@echo "\t[MKDIR] ${IMGROOT}/Extra/Themes/Default"
	@mkdir -p ${IMGROOT}/Extra/Themes/Default				
	@echo "\t[MKDIR] ${IMGROOT}/usr/bin"
	@mkdir -p ${IMGROOT}/usr/bin
	@if [ -e "$(IMGSKELROOT)" ]; then				\
		@echo "\t[CP] ${IMGROOTSKEL} ${IMGROOT}"		\
		@cp -R -f "${IMGSKELROOT}"/* "${IMGROOT}";		\
	fi;								  
	@cp -f ${SYMROOT}/i386/cdboot ${CDBOOT}
	@cp -f ${SYMROOT}/i386/modules/* ${IMGROOT}/Extra/modules
	@cp -f ${SRCROOT}/artwork/themes/default/* ${IMGROOT}/Extra/Themes/Default
	@cp -f ${SYMROOT}/i386/boot ${IMGROOT}/usr/standalone/i386
	@cp -f ${SYMROOT}/i386/boot0 ${IMGROOT}/usr/standalone/i386
	@cp -f ${SYMROOT}/i386/boot0hfs ${IMGROOT}/usr/standalone/i386
	@cp -f ${SYMROOT}/i386/boot0md ${IMGROOT}/usr/standalone/i386
	@cp -f ${SYMROOT}/i386/boot1h ${IMGROOT}/usr/standalone/i386
	@cp -f ${SYMROOT}/i386/boot1f32 ${IMGROOT}/usr/standalone/i386
ifdef CONFIG_FDISK440
	@cp -f ${SYMROOT}/i386/fdisk440 ${IMGROOT}/usr/bin
endif
ifdef CONFIG_BDMESG
	@cp -f ${SYMROOT}/i386/bdmesg ${IMGROOT}/usr/bin    
endif
	@echo "\t[HDIUTIL] ${ISOIMAGE}"
	@hdiutil makehybrid -iso -joliet -hfs -hfs-volume-name \
		${CDLABEL} -eltorito-boot ${CDBOOT} -no-emul-boot -ov -o   \
		"${ISOIMAGE}" ${IMGROOT} -quiet 		  	  
	@echo "\t[GZ] ${DISTFILE}.tgz"
	@rm -f ${DISTFILE}.tar.gz
	@cd ${SYMROOT} && tar -cf ${DISTFILE}.tar ${DISTROOT}
	@gzip --best ${DISTFILE}.tar
	@mv ${DISTFILE}.tar.gz ${DISTFILE}.tgz

clean-local:
	@if [ -f "$(HEADER_VERSION)" ];then echo "\t[RM] $(HEADER_VERSION)"; fi
	@if [ -f "$(SRCROOT)/revision" ];then echo "\t[RM] $(SRCROOT)/revision"; fi
	@rm -f $(HEADER_VERSION) $(SRCROOT)/revision

distclean-local:
	@if [ -d "$(OBJROOT)" ];then echo "\t[RMDIR] $(OBJROOT)"; fi
	@if [ -d "$(SYMROOT)" ];then echo "\t[RMDIR] $(SYMROOT)"; fi
	@if [ -d "$(DSTROOT)" ];then echo "\t[RMDIR] $(DSTROOT)"; fi
	@if [ -d "$(SRCROOT)/i386/modules/module_includes" ];then \
		echo "\t[RMDIR] $(SRCROOT)/i386/modules/module_includes"; \
	 fi
	@if [ -f "$(SRCROOT)/auto.conf" ];then echo "\t[RM] $(SRCROOT)/auto.conf"; fi
	@if [ -f "$(SRCROOT)/autoconf.h" ];then echo "\t[RM] $(SRCROOT)/autoconf.h"; fi
	@if [ -f "$(SRCROOT)/autoconf.inc" ];then echo "\t[RM] $(SRCROOT)/autoconf.inc"; fi

	@rm -rf $(OBJROOT) $(SYMROOT) $(DSTROOT)        \
            $(SRCROOT)/i386/modules/module_includes \
            $(SRCROOT)/auto.conf                    \
            $(SRCROOT)/autoconf.h                   \
            $(SRCROOT)/autoconf.inc

pkg installer: all
	${SRCROOT}/package/buildpkg.sh ${SYMROOT}/package;
	@echo "\t[GZ] ${DISTFILE}.pkg"
	@gzip --best ${DISTFILE}.pkg

dmg: all
	${SRCROOT}/package/slimpkg.sh ${SYMROOT}/package;
	${SRCROOT}/package/builddmg.sh ${SYMROOT}/package;

$(SYMROOT)/i386/vers.h: version
	@echo "#define I386BOOT_VERSION \"5.0.132\"" > $@
	@echo "#define I386BOOT_BUILDDATE \"`date \"+%Y-%m-%d %H:%M:%S\"`\"" >> $@
	@echo "#define I386BOOT_CHAMELEONVERSION \"`cat version`\"" >> $@
	@echo "#define I386BOOT_CHAMELEONREVISION \"`svnversion -n | tr -d [:alpha:]`\"" >> $@


.PHONY: $(SYMROOT)/i386/vers.h
.PHONY: config
.PHONY: clean
.PHONY: image
.PHONY: pkg
.PHONY: installer
.PHONY: dmg
