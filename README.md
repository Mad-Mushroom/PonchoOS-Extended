# PonchoOS-Extended

A OS build ontop of Poncho's Tutorial Series.


ToDo:

- [x] Bootloader
- [x] GOP
- [x] paging
- [x] Keyboard Input
- [x] Mouse
- [x] AHCI
- [ ] Window System
- [ ] Terminal App
- [ ] Clock App
- [ ] Desktop
- [ ] FileSystem
- [ ] Background Image
- [ ] Shutdown / Reboot


syntax on
set number
set mouse=a
set path+=**
set noshowmode
set laststatus=2

call plug#begin('~/.vim/plugged')

Plug 'itchyny/lightline'

call plug#end()

let g:lightline = {
      \ 'colorscheme': 'wombat',
      \ }
