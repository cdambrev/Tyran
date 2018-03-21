
document.onreadystatechange = () => {
  if (document.readyState === 'complete') {
    document.getElementById("menu").innerHTML = "<label class='labelmenu'><div class='tyranCheck'><input type='checkbox' id='catTyran' checked> Tyran </div></label><label class='labelmenu'><div class='iaCheck'><input type='checkbox' id='catIA' checked> Intelligence Artificielle </div></label><label class='labelmenu'><div class='hudCheck'><input type='checkbox' id='catHUD' checked> HUD </div></label><br><label class='labelmenu'><div class='poursuiteCheck'><input type='checkbox' id='catPoursuite' checked> Poursuite </div></label>";
	

    document.getElementById("catTyran").onclick = function(){
    	elements = document.getElementsByClassName("tyran");
    	for (var i = 0; i < elements.length; i++) {
        elements[i].classList.toggle("hidden");
   		}
    }


    document.getElementById("catIA").onclick = function(){
    	elements = document.getElementsByClassName("ia");
    	for (var i = 0; i < elements.length; i++) {
        elements[i].classList.toggle("hidden");
   		}
    }

    document.getElementById("catHUD").onclick = function(){
    	elements = document.getElementsByClassName("hud");
    	for (var i = 0; i < elements.length; i++) {
        elements[i].classList.toggle("hidden");
   		}
    }
	
	document.getElementById("catPoursuite").onclick = function(){
    	elements = document.getElementsByClassName("poursuite");
    	for (var i = 0; i < elements.length; i++) {
                   elements[i].classList.toggle("hidden");
		}
    }

}
  
};