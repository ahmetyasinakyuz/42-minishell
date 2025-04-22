/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/22 11:54:52 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Belirtilen anahtarın çevre değişkenleri listesinde olup olmadığını kontrol eder.
 * 
 * @param key Kontrol edilecek anahtar.
 * @param vars Çevre değişkenleri listesinin adresi.
 * @return Anahtar listede bulunuyorsa 1, bulunmuyorsa 0 döner.
 */
int	is_in_vars(char *key, t_vars **vars)
{
	t_vars	*temp;

	if (!vars || !*vars)
		return (0);
	temp = *vars;
	while (temp)
	{
		if (temp->key && ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (1);
		temp = temp->next;
	}
	return (0);
}

/**
 * Belirtilen anahtarın değerini çevre değişkenleri listesinden alır.
 * 
 * @param key İstenen değişkenin anahtarı.
 * @param vars Çevre değişkenleri listesinin adresi.
 * @return Değişkenin değeri veya bulunamazsa NULL.
 */
char	*get_var(char *key, t_vars **vars)
{
	t_vars	*temp;

	if (!vars || !*vars)
		return (NULL);
	temp = *vars;
	while (temp)
	{
		if (temp->key && ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

/**
 * Belirtilen çevre değişkeninin değerini alır. Önce iç değişkenler listesini,
 * bulamazsa sistem çevre değişkenlerini kontrol eder.
 * 
 * @param env_var Değeri istenen çevre değişkeninin adı.
 * @param vars İç değişkenler listesinin adresi.
 * @return Değişkenin değerinin kopyası veya bulunamazsa boş string.
 */
char	*get_env_value(char *env_var, t_vars **vars)
{
	char	*env_value;
	char	*result;

	if (is_in_vars(env_var, vars))
	{
		env_value = get_var(env_var, vars);
		if (!env_value || !*env_value)
			return (ft_strdup(""));
		result = ft_strdup(env_value);
		return (result);
	}
	env_value = getenv(env_var);
	if (!env_value)
		return (ft_strdup(""));
	result = ft_strdup(env_value);
	return (result);
}

/**
 * Çevre değişkeni yerleştirmesi için metinde parçaları birleştirir.
 * 
 * @param result Üzerinde işlem yapılacak orijinal metin.
 * @param i Değişken adının başladığı konum.
 * @param env_value Yerleştirilecek değişkenin değeri.
 * @param j Değişken adının uzunluğu.
 * @return Değişken değeri yerleştirilmiş yeni metin veya hata durumunda NULL.
 */
char	*join_env_parts(char *result, int i, char *env_value, int j)
{
	char	*temp;
	char	*final_result;

	temp = ft_substr(result, 0, i);
	if (!temp)
	{
		free(env_value);
		free(result);
		return (NULL);
	}
	final_result = ft_strjoin(temp, env_value);
	free(temp);
	free(env_value);
	if (!final_result)
	{
		free(result);
		return (NULL);
	}
	temp = ft_strjoin(final_result, &result[i + j + 1]);
	free(final_result);
	free(result);
	return (temp);
}

/**
 * Metindeki çevre değişkeni referansını ($ENV_VAR) gerçek değeriyle değiştirir.
 * 
 * @param result İşlenecek metin.
 * @param i '$' işaretinin konumu.
 * @param vars Çevre değişkenleri listesi.
 * @return Değişken yerleştirilmiş yeni metin.
 */
char	*replace_env_var(char *result, int i, t_vars **vars)
{
	char	*env_var;
	int		j;
	char	*env_value;

	j = 1;
	if (ft_isdigit(result[i + j]))
		j++;
	else
		while (result[i + j] && (ft_isalnum(result[i + j]) || result[i
					+ j] == '_'))
			j++;
	env_var = ft_substr(result, i + 1, j - 1);
	env_value = get_env_value(env_var, vars);
	free(env_var);
	result = join_env_parts(result, i, env_value, j - 1);
	return (result);
}

/**
 * Değişkenler listesine yeni bir statik değişken ekler.
 * Eğer anahtar zaten listede varsa, işlem yapılmaz.
 * 
 * @param vars Değişkenler listesinin adresi.
 * @param key Yeni değişkenin anahtarı.
 * @param value Yeni değişkenin değeri.
 */
void	add_static_var(t_vars **vars, char *key, char *value)
{
	t_vars	*new_var;
	t_vars	*temp;

	if (is_in_vars(key, vars))
		return;
	new_var = malloc(sizeof(t_vars));
	if (!new_var)
		return;
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = NULL;
	new_var->prev = NULL;
	if (!*vars)
	{
		*vars = new_var;
		return;
	}
	temp = *vars;
	while (temp->next)
		temp = temp->next;
	temp->next = new_var;
	new_var->prev = temp;
}

/**
 * Checks if a string is in the format of a variable assignment (key=value)
 * 
 * @param str String to check
 * @return 1 if it's a variable assignment, 0 otherwise
 */
int	is_var_assignment(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (str[i] == '=');
}
