attribute vec4 coord;
varying vec2 texcoord;

void main(void) {
	texcoord = coord.zw;
	gl_Position = vec4(coord.xy, 0, 1);
}
