
var Title = new Array();
Title['/home'] = 'Qem - Home';
Title['/about'] = 'Qem - About';
Title['/doc'] = 'Qem - Documentation';
Title['/downloads'] = 'Qem - Downloads';

var Pages = new Array('about', 'doc', 'downloads', 'home');

function onPageLoad(id)
{
	for (i=0; i<Pages.length; i++) 
	{ 
		if($("#nav-"+Pages[i])) 
			$("#nav-"+Pages[i]).addClass('page_item');
		$("#nav-"+Pages[i]).removeClass('current_page_item');
	}
	
	if($("#nav-"+id.substring(1, id.length))) 
	{ 
		$("#nav-"+id.substring(1, id.length)).removeClass('page_item');
		$("#nav-"+id.substring(1, id.length)).addClass('current_page_item'); 
	}
}

function pageload(hash) 
{
	if($.inArray(hash.substring(1, hash.length), Pages) > -1) 
	{
		var cache = $.jCache.getItem(hash);
		if(cache) 
		{
			$("#section").html(cache);
			document.title = Title[hash];
			onPageLoad(hash);
		} 
		else 
		{
			$.ajax({ type: "GET", url: hash.substring(1, hash.length)+'.html', 
				success: function(data){
					$.jCache.setItem(hash, data);
					if(hash == "/home")
					{
						$("#section").html(data);
						document.title = Title[hash];
					}
				},
				error: function(){
					$.historyLoad('/404');
				},
				beforeSend: function() {
					$("#section").html("<img src='images/loader.gif' alt='Loading' />");
				}
			});
		}
	}
}

var historyInitiated = false;

$(document).ready(function()
	{
		if(!historyInitiated)
		{
			historyInitiated = true;
			$.historyInit(pageload);
			preLoadPages();
			
			// sélection de tous les liens a ayant l'attribut rel égal à history (valeur arbitraire)
			$(".history").click(function()
			{
				//suppression du mot cle history, pour que les liens ne soient surchargés qu'une seule fois
				this.rel = this.rel.replace(/history/, '');
				// mise à jour de l'ancre
				var hash = this.href;
				// suppression du caractère #
				hash = hash.replace(/^.*#/, '');
				// chargement dans l'historique et appel de pageload
				$.historyLoad(hash);
				// trés important : désactivation du clic du lien a
				return false;
			});
		}
	}
);

// Précharge en cache les pages du site pour une navigation plus fluide 
function preLoadPages() 
{
	for (i=0; i<Pages.length; i++) 
	{
		var hash = "/"+Pages[i];
		pageload(hash);
	}
}
