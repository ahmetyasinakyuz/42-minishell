/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 04:21:10 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 18:33:38 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Değişken listesine yeni bir değişken ekler.
 * Bu fonksiyon, verilen anahtar (key) ve değer (value) ile yeni bir değişken düğümü oluşturur 
 * ve bu düğümü değişken listesine ekler. Liste boşsa, yeni değişken liste başlangıcı olur.
 * 
 * @param vars Değişkenin ekleneceği liste referansı
 * @param key Değişken adı
 * @param value Değişken değeri
 */
void	add_var(t_vars **vars, char *key, char *value)
{
	t_vars	*new_var;
	t_vars	*temp;

	new_var = (t_vars *)malloc(sizeof(t_vars));
	if (!new_var)
		return ;
	new_var->key = key;
	new_var->value = value;
	new_var->next = NULL;
	new_var->prev = NULL;
	if (!*vars)
	{
		*vars = new_var;
		return ;
	}
	temp = *vars;
	while (temp && temp->next)
		temp = temp->next;
	temp->next = new_var;
	new_var->prev = temp;
}

/**
 * Bir metnin değişken tanımlama formatında olup olmadığını kontrol eder.
 * Bu fonksiyon, verilen metinde bir eşittir (=) işareti olup olmadığını kontrol ederek
 * metnin bir değişken tanımı olup olmadığını belirler (örn: "ISIM=Ahmet").
 * 
 * @param str Kontrol edilecek metin
 * @return 1: Metin bir değişken tanımı, 0: Değil
 */
int	is_declare(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		return (1);
	return (0);
}

/**
 * Bir değişkenin zaten tanımlı olup olmadığını kontrol eder.
 * Bu fonksiyon, verilen anahtarın değişken listesinde var olup olmadığını kontrol eder.
 * Eğer varsa, değişkenin değerini günceller ve eski anahtar belleğini serbest bırakır.
 * 
 * @param vars Kontrol edilecek değişken listesi
 * @param key Kontrol edilecek değişken adı
 * @param value Değişken zaten tanımlıysa atanacak yeni değer
 * @return 1: Değişken zaten tanımlı ve güncellendi, 0: Değişken tanımlı değil
 */
int	is_alraday_declared(t_vars **vars, char *key, char *value)
{
	t_vars	*temp;

	temp = *vars;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
		{
			free(temp->value);
			temp->value = value;
			free(key);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

/**
 * Metinden değişken tanımlarını bulur ve değişken listesine ekler.
 * Bu fonksiyon, verilen metni inceleyerek değişken tanımlama formatında olup olmadığını kontrol eder.
 * Eğer bir değişken tanımı bulunursa, bu değişkeni anahtar ve değer olarak ayırıp listeye ekler.
 * 
 * @param str İncelenecek metin
 * @param vars Değişken ekleme işlemi yapılacak liste
 */
void	found_var(char *str, t_vars **vars)
{
	char	*key;
	char	*value;
	int		i;

	if (!is_declare(str))
		return ;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	value = ft_strdup(str + i + 1);
	if (is_alraday_declared(vars, key, value))
		return ;
	add_var(vars, key, value);
}

/**
 * Dolar işareti karakterlerini işler.
 * Bu fonksiyon, metindeki dolar işaretlerini işler. Eğer dolar işaretinden sonra alfabetik bir karakter
 * varsa, çevresel değişken değerini metin içerisine yerleştirir. Aksi takdirde, dolar işareti
 * ve sonrasındaki alfanumerik olmayan karakterleri kaldırır.
 * 
 * @param result İşlenecek metin
 * @param i İşlenecek metin içindeki konum referansı
 * @param vars Değişken listesi
 * @return İşlenmiş metin
 */
char	*handle_dollar_char(char *result, int *i, t_vars **vars)
{
	int		j;
	char	*temp;

	if (result[*i + 1] && ft_isalpha(result[*i + 1]))
	{
		result = replace_env_var(result, *i, vars);
		*i = -1;
	}
	else
	{
		j = *i;
		while (result[j] && !ft_isalpha(result[j]))
			j++;
		temp = ft_strdup(result);
		free(result);
		result = ft_strjoin(ft_substr(temp, 0, *i), temp + j);
		free(temp);
		*i = -1;
	}
	return (result);
}
