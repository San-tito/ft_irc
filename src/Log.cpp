/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:57:02 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/08 14:15:39 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

Log::Log(void) : start_time_(std::time(0))
{
}

Log::~Log(void)
{
}

Log::Notice::Notice(void)
{
}

Log::Notice::~Notice(void)
{
	std::cout << buffer_.str() << '\n';
}

Log::Info::Info(void)
{
}

Log::Info::~Info(void)
{
	char	str[64];

	strftime(str, 64, "%Y-%m-%dT%H:%M:%S%z", localtime(&start_time_));
	std::cout << "[" << str << "] " << buffer_.str() << '\n';
}

Log::Err::Err(void)
{
}

Log::Err::~Err(void)
{
	char	str[64];

	strftime(str, 64, "%Y-%m-%dT%H:%M:%S%z", localtime(&start_time_));
	std::cerr << "[" << str << "] " << buffer_.str() << '\n';
}
