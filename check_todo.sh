#! /bin/sh

# TODO: ONLY remove this file when the following command doesn't print anything anymore
# (except this file) of course
grep --color=always --exclude-dir=.git --exclude-dir=libft -inr -- 'TODO[:]'
