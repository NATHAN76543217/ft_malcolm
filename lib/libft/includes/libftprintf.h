/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 05:36:15 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:18:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIBFTPRINTF_H
# define FT_LIBFTPRINTF_H
# include <stdarg.h>
//Pour la va_list

# define RESET			"\033[0m"
# define BLACK			"\033[30m"
# define RED			"\033[31m"
# define GREEN			"\033[32m"
# define YELLOW			"\033[33m"
# define BLUE			"\033[34m"
# define MAGENTA		"\033[35m"
# define CYAN			"\033[36m"
# define WHITE			"\033[37m"
# define ORANGE			"\033[38;5;214m"
# define BOLDORANGE		"\033[1m\033[38;5;214m"
# define BOLDBLACK		"\033[1m\033[30m"
# define BOLDRED		"\033[1m\033[31m"
# define BOLDGREEN		"\033[1m\033[32m"
# define BOLDYELLOW		"\033[1m\033[33m"
# define BOLDBLUE		"\033[1m\033[34m"
# define BOLDMAGENTA	"\033[1m\033[35m"
# define BOLDCYAN		"\033[1m\033[36m"
# define BOLDWHITE		"\033[1m\033[37m"

typedef struct	s_pf
{
	va_list			*ap;
	enum e_boolean	fzero;
	enum e_boolean	fstars;
	enum e_boolean	fmoins;
	enum e_boolean	fprecision;
	enum e_boolean	fdiese;
	enum e_boolean	fplus;
	enum e_boolean	fspace;
	enum e_boolean	fapostrophe;
	int				vprecision;
	int				width;
	int				whitespace;
	int				zero;
	char			specifier;
	int				length;
}				t_pf;

int				ft_printf(const char *str,
				...) __attribute__((format(printf,1,2)));
int				pf_conv(t_pf *tpf, const char *format);
void			pf_initlst(va_list *ap, t_pf *tpf);
void			disp_char(t_pf *tpf);
void			disp_str(t_pf *tpf);
void			disp_int(t_pf *tpf, char *base);
void			disp_ptr(t_pf *tpf, char *base);
void			disp_percent(t_pf *tpf);
int				pf_prec(t_pf *tpf, long val, int argsize);
void			length_calc(t_pf *tpf, int count, ...);
void			init_tab(void);

#endif
