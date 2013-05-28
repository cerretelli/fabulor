/*
 * core.c
 * 
 * Copyright 2013 Jamie Cerretelli <jamiecerretelli@gmail.com>
 *
 */
 
#include "core.h"

gint init_core(_widgets *widgets)
{
	_cert *cs;
	gint fd = 0, n = 0;
	guchar buf[1024];
	
	cs = g_malloc(sizeof(_cert));
	
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	
	fd = open("/dev/random", O_RDONLY);
	n = read(fd, buf, sizeof buf);
	close(fd);
	srand((guint)n);
	
	if(generate_cert(widgets, cs, n) != 0)
	{
		log_message(widgets, "[!] Certificate generation failed");
		return 1;
	}
	
	// clean up
	clean_up(cs);
	
	return 0;
}

//
gint generate_cert(_widgets *widgets, _cert *cs, gint entropy)
{
	gchar *dsize;
	gchar buff[1024*4];
	gint size = 0;
	gint bio_return = 0;
	
	log_message(widgets, "Generating certificate...");
	
	if((cs->rsa = RSA_new()) == 0)
	{
		log_message(widgets, "[!] RSA_new failed");
		return 1;
	}
	
	// generate password
	cs->password = g_malloc(sizeof(gchar) * 20);
	generate_string(cs->password, 20);
	
	// post password to entry
	gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(widgets->buffer_password), cs->password, 20);
	
	RAND_add(cs->password, sizeof cs->password, entropy);
	
	dsize = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->cbo_size));
	
	if(dsize == NULL)
	{
		log_message(widgets, "[!] Modulus size empty");
		return 1;
	}
	
	if(g_strcmp0(dsize, "512") == 0) {	size = 512;	}
	else if(g_strcmp0(dsize, "1024") == 0) { size = 1024; }
	else if(g_strcmp0(dsize, "2048") == 0) { size = 2048; }
	else if(g_strcmp0(dsize, "4096") == 0) { size = 4096; }
	
	log_message(widgets, g_strdup_printf("Modulus size: %s", dsize));
	
	log_message(widgets, "Generating key");
		
	if((cs->rsa = RSA_generate_key(size, generate_odd_number(), 0, 0)) == NULL)
	{
		log_message(widgets, "[!] RSA_generate_key failed");
		return 1;
	}
	
	if(!RSA_check_key(cs->rsa))
	{
		log_message(widgets, "[!] RSA_check_key failed");
		return 1;
	}
	
	log_message(widgets, "RSA check key: Passed");
	
	//
	//
	cs->bio = BIO_new(BIO_s_mem());
	
	if((cs->pkey = EVP_PKEY_new()) == 0)
	{
		log_message(widgets, "[!] EVP_PKEY_new failed");
		return 1;
	}
	
	if(!EVP_PKEY_assign_RSA(cs->pkey, cs->rsa))
	{
		log_message(widgets, "[!] EVP_PKEY_assign_RSA failed");
		return 1;
	}
	
	log_message(widgets, "Writing Public key");
	if(!PEM_write_bio_RSAPublicKey(cs->bio, cs->rsa))
	{
		log_message(widgets, "[!] PEM_write_bio_RSAPublicKey failed");
		return 1;
	}
	
	bio_return = BIO_read(cs->bio, buff, sizeof(buff)-1);
	if(bio_return < 1)
	{
		log_message(widgets, "[!] BIO_read failed");
		return 1;
	}
	
	// write public key to window
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(widgets->buffer_public), buff, bio_return);
	
	//
	// Private key
	log_message(widgets, "Writing Private key");
	
	if(!PEM_write_bio_PKCS8PrivateKey(cs->bio, cs->pkey, EVP_des_ede3_cbc(), NULL, 0, 0, cs->password))
	{
		//log_message(widgets, "[!] PEM_write_bio_PKCS8PrivateKey");
		log_message(widgets, g_strdup_printf("[!] PEM_write_bio_PKCS8PrivateKey failed : %s", (gchar *)ERR_reason_error_string(ERR_get_error())));
		return 1;
	}
	
	bio_return = BIO_read(cs->bio, buff, sizeof(buff));
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(widgets->buffer_private), buff, bio_return);
	
	log_message(widgets, "Keys generated!");
	
	g_free(dsize);
	
	return 0;
}

//
gint generate_odd_number()
{
	int i = 0, min = 1, max = 999999;
	
	while(1)
	{
		i = (rand() % (max + 1 - min)) + min;
		if(i % 2 != 0) return i;
		continue;
	}
	return min;
}

//
gchar *generate_string(gchar *out, gint length)
{
	int i;
	static const gchar c[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							"abcdefghijklmnopqrstuvwxyz"
							"0123456789"
							"!\"£$%^&*()_+{}~@?><:[]#'/"
							".,-=`¬_+";
							
	for(i = 0; i < length; i++)
	{
		out[i] = c[rand() % sizeof(c)];
	}
	
	return out;
}

//
void clean_up(_cert *cs)
{
	EVP_PKEY_free(cs->pkey);
	g_free(cs->password);
	g_free(cs);
}

//
gint encrypt_file(_widgets *widgets)
{
	return 0;
}

//
gint decrypt_file(_widgets *widgets)
{
	return 0;
}
