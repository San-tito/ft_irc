/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sig.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/08 00:03:24 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_HPP
# define SIG_HPP

# include "Log.hpp"
# include <signal.h>
# include <string.h>
# include <string>
# include <unistd.h>

class Sig
{
  public:
	static bool quit;
	static void SigHandler(int sig);
	static void Init(void);
	static void Exit(void);
};

#endif /* SIG_HPP */
