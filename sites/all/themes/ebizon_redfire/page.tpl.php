<?php
// $Id: page.tpl.php,v 1.1.2.3 2009/01/11 18:17:25 ebizondrupalservices Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
 <head>
<?php global $base_url;?>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
    <script>
  		$(document).ready(function () {
  			$('.sticky-table td:first-child').addClass('borderright');
  			$('.sticky-table td:nth-child(2)').addClass('borderleft');
  			$('.sticky-table th:nth-child(2) a').addClass('changemargin');
  			$('.sticky-table th:nth-child(3)').addClass('centerme');
  			$('.sticky-table td:nth-child(3)').addClass('changecolor');
  			$('.tableHeader-processed th:first-child').addClass('borderright');
  			$('.tableHeader-processed th:nth-child(2)').addClass('borderleft');
  			$('.tableHeader-processed a:first').addClass('negmarg');
			});
 	 </script>
  </head>

  <body class="<?php print $body_classes;?>">
    <div id="layout">
      <div id="header">
				<div id="logo">
					<a href="/"><img src="/sites/all/themes/ebizon_redfire/img/jibelogo.jpg" width="80px"/></a>
              
       		 <?php
         		 // Prepare header
         		 /*$site_fields = array();
         		 if ($site_name) {
         		   $site_fields[] = check_plain($site_name);
        		  }
         		 if ($site_slogan) {
        		    $site_fields[] = check_plain($site_slogan);
        		  }
        		  $site_title = implode(' ', $site_fields);
        		  $site_fields[0] = '<span>'. $site_fields[0] .'</span>';
        		  $site_html = implode(' ', $site_fields);
		
        		  if ($logo || $site_title) {
        		    print '<h1><a href="'. check_url($base_path) .'" title="'. $site_title .'">';
        		    if ($logo) {
         		     //print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" id="logo" />';
          		  }
          		  print $site_html .'</a></h1>';
         		 }*/
        		?>
     		</div>
        <hr class="noscreen" />   
          
              
        <div id="navigation">
        <ul>
          <li><a href="/<?php print_r($user->name);?>" id="cab">FILING <br/>CABINET</a></li>
          <li><a href="/<?php print_r($user->name);?>-wiki" id="wiki">WIKI</a></li>
        </ul>
        </div>
        <hr class="noscreen" />  
    
      </div>
      
        <div class="rbroundbox">
	      
		          <div class="rbcontent">
          			
          		<div class="contents">
                          <?php print $messages;?>
	  <?php print $tabs;?>
	  
              </div>
              <div id="panel-right"><div id="panel-right-inside">
                <?php print $search_box; ?>
              </div></div>	
          			
        		  </div>
	         
        </div>
          
	<div id="main">
         
         <div class="mainpage">         
         <?php print $content;?> 
	       </div>
              <div class="rightblock">
              <?php print $right;?>
              </div>
        </div>
        </div>
        <!--<div id="footer">
        <div id="footer-inside">
        
        </div></div>-->
       <?php print $closure ?>
  </body>
</html>
