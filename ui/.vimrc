set et ts=4 sw=4

highlight YcmErrorSection guibg=#ffa500
let g:ycm_confirm_extra_conf = 0

augroup templates
  au!
  " read in template files
  autocmd BufNewFile *.[ch] silent! execute '0r $HOME/vimfiles/templates/skeleton.'.expand("<afile>:e")

  " parse special text in the templates after the read
  autocmd BufNewFile * %substitute#\[:VIM_EVAL:\]\(.\{-\}\)\[:END_EVAL:\]#\=eval(submatch(1))#ge
augroup END

