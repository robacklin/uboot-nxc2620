
include $(TOPDIR)/config.mk
include $(TOPDIR)/nand_spl/board/$(BOARDDIR)/config.mk

LDSCRIPT= $(TOPDIR)/nand_spl/board/$(BOARDDIR)/u-boot.lds
LDFLAGS	= -EL -Bstatic -T $(LDSCRIPT) -Ttext $(TEXT_BASE) $(PLATFORM_LDFLAGS) -G 0 -n -nostdlib
CFLAGS	+= -DCONFIG_NAND_SPL -Os -G 0 -mno-abicalls -fno-pic -mips32 -mlong-calls -pipe

SOBJS	= start.o lowlevel_init.o
COBJS	= nxc2600.o nand.o time.o serial.o cpu.o memsize.o

SRCS	:= $(addprefix $(obj),$(SOBJS:.o=.S) $(COBJS:.o=.c))
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
__OBJS	:= $(SOBJS) $(COBJS)
LNDIR	:= $(OBJTREE)/nand_spl/board/$(BOARDDIR)

nandobj	:= $(OBJTREE)/nand_spl/

ALL	= $(nandobj)u-boot-spl $(nandobj)u-boot-spl.bin

all:	$(obj).depend $(ALL)

$(nandobj)u-boot-spl.bin:	$(nandobj)u-boot-spl
	$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

$(nandobj)u-boot-spl:	$(OBJS)
	cd $(LNDIR) && $(LD) $(LDFLAGS) $$UNDEF_SYM $(__OBJS) \
		-Map $(nandobj)u-boot-spl.map \
		-o $(nandobj)u-boot-spl

# create symbolic links for common files

# from cpu directory

$(obj)lowlevel_init.S:
	@rm -f $(obj)lowlevel_init.S
	ln -s $(SRCTREE)/board/icnexus/nxc2600/lowlevel_init.S $(obj)lowlevel_init.S

$(obj)cpu.c:
	@rm -f $(obj)cpu.c
	ln -s $(SRCTREE)/cpu/mips/cpu.c $(obj)cpu.c

$(obj)start.S:
	@rm -f $(obj)start.S
	ln -s $(SRCTREE)/cpu/mips/start.S $(obj)start.S

$(obj)nxc2600.c:
	@rm -f $(obj)nxc2600.c
	ln -s $(SRCTREE)/board/icnexus/nxc2600/nxc2600.c $(obj)nxc2600.c

$(obj)nand.c:
	@rm -f $(obj)nand.c
	ln -s $(SRCTREE)/board/icnexus/nxc2600/nand.c $(obj)nand.c

$(obj)time.c:
	@rm -f $(obj)time.c
	ln -s $(SRCTREE)/lib_mips/time.c $(obj)time.c

$(obj)serial.c:
	@rm -f $(obj)serial.c
	ln -s $(SRCTREE)/board/icnexus/nxc2600/serial.c $(obj)serial.c


$(obj)memsize.c:
	@rm -f $(obj)memsize.c
	ln -s $(SRCTREE)/common/memsize.c $(obj)memsize.c

#########################################################################

$(obj)%.o:	$(obj)%.S
	$(CC) $(CFLAGS) -D__ASSEMBLY__ -c -o $@ $<

$(obj)%.o:	$(obj)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
