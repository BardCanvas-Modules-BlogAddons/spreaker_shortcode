<?php
/**
 * Functions for spreaker shortcode handling
 *
 * @package    BardCanvas
 * @subpackage spreaker_shortcode
 * @author     https://www.spreaker.com - Ported by Alejandro Caballero - lava.caballero@gmail.com
 *
 */

function process_spreaker_shortcode($attributes)
{
    global $this_module;
    
    // Get the shortcode type
    $type = !empty($attributes['type']) ? $attributes['type'] : null;
    if (!$type) {
        // Backward compatibility (previous versions of the plugin didn't require the type attribute)
        return spreaker_get_player_by_legacy_config($attributes);
    }
    
    switch($type) {
        case 'mini':
        case 'standard':
            return spreaker_get_player_by_legacy_config($attributes);
        
        case 'player':
            return spreaker_get_player($attributes);
        
        default:
            return $this_module->language->type_unsupported;
    }
}

function spreaker_get_player_by_legacy_config($attributes)
{
    global $this_module;
    
    // Get shortcode params
    $params = array(
        'type'        => 'mini',
        'width'       => '100%',
        'autoplay'    => 'false',
        'episode_id'  => null,
        'show_id'     => null,
        'user_id'     => null
    );
    foreach($attributes as $key => $val) if( empty($params[$key]) ) $params[$key] = $val;
    
    // Ensure the resource id has been provided
    if (empty($params['user_id']) && empty($params['episode_id']) && empty($params['show_id'])) {
        return $this_module->language->missing_resource;
    }
    
    // Generate the resource value
    $resource = '';
    
    if (!empty($params['user_id'])) {
        $resource = 'user_id=' . $params['user_id'];
    } else if (!empty($params['show_id'])) {
        $resource = 'show_id=' . $params['show_id'];
    } else if (!empty($params['episode_id'])) {
        $resource = 'episode_id=' . $params['episode_id'];
    }
    
    // Convert attributes to new ones
    $config = array(
        'resource' => $resource,
        'width'    => $params['width'],
        'height'   => $params['type'] === 'mini' ? '140px' : '200px',
        'autoplay' => $params['autoplay'],
        'theme'    => 'dark'
    );
    
    return spreaker_get_player($config);
}

function spreaker_get_player($attributes)
{
    global $this_module;
    
    // Get shortcode params
    $params = array(
        'resource'            => null,
        'width'               => '100%',
        'height'              => '200px',
        'theme'               => null,
        'cover'               => null,
        'playlist'            => null,
        'playlist-continuous' => null,
        'playlist-loop'       => null,
        'playlist-autoupdate' => null,
        'chapters-image'      => null,
        'autoplay'            => null,
        'live-autoplay'       => null
    );
    foreach($attributes as $key => $val) if( empty($params[$key]) ) $params[$key] = $val;
    
    // Ensure the resource has been provided
    if (empty($params['resource'])) {
        return $this_module->language->missing_resource;
    }
    
    // Validate the resource
    preg_match('/^(user_id|show_id|episode_id)=(\d+)$/', strtolower(trim($params['resource'])), $matches);
    if (!$matches) {
        return $this_module->language->invalid_resource;
    }
    
    $resource_type = $matches[1];
    $resource_id   = $matches[2];
    
    // Get player size
    $width  = spreaker_get_dimension($params['width'], '100%');
    $height = spreaker_get_dimension($params['height'], '200px');
    
    // Build the url
    $player_url = spreaker_get_url('https://widget.spreaker.com/player', array(
        "$resource_type"      => $resource_id,
        'theme'               => $params['theme'],
        'cover_image_url'     => $params['cover'],
        'playlist'            => $params['playlist'],
        'playlist-continuous' => $params['playlist-continuous'],
        'playlist-loop'       => $params['playlist-loop'],
        'playlist-autoupdate' => $params['playlist-autoupdate'],
        'chapters-image'      => $params['chapters-image'],
        'autoplay'            => $params['autoplay'],
        'live-autoplay'       => $params['live-autoplay'],
    ));
    
    return '<iframe src="' . $player_url . '" width="' . $width . '" height="' . $height . '" frameborder="0"></iframe>';
}

function spreaker_get_dimension($input, $default = '100%')
{
    if (empty($input)) {
        return $default;
    }
    
    // Cleanup
    $input = strtolower(trim($input));
    
    // Accept a value without unit (px by default)
    if (preg_match('/^\d+$/', $input)) {
        return $input . 'px';
    } elseif (!preg_match('/^\d+(%|px)$/', $input)) {
        return $default;
    } else {
        return $input;
    }
}

function spreaker_get_url($base_url, $params = array())
{
    // Filter out empty params
    foreach ($params as $key => $value) {
        if ($value === null || $value === '') {
            unset($params[$key]);
        }
    }
    
    // Covert boolean to strings
    foreach ($params as $key => $value) {
        if (is_bool($value)) {
            $params[$key] = $value ? 'true' : 'false';
        }
    }
    
    return $base_url . '?' . http_build_query($params);
}
