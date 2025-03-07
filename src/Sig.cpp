/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sig.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:57:02 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 22:34:06 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sig.hpp"

bool Sig::quit(false);

void Sig::SigHandler(int sig)
{
	std::cout << "Got signal \"" << strsignal(sig) << "\" ...\n";
	quit = true;
}

void Sig::Init(void)
{
	signal(SIGINT, SigHandler);
	signal(SIGQUIT, SigHandler);
	signal(SIGTERM, SigHandler);
}

void Sig::Exit(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}
