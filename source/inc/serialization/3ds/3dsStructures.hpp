/*
 * 	Copyright (c) 2010-2011, Romuald CARI
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *		* Neither the name of the <organization> nor the
 *		  names of its contributors may be used to endorse or promote products
 *		  derived from this software without specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL Romuald CARI BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once

#include <PantinExport.hpp>

namespace Pantin { namespace serialization { namespace ThreeDS {

struct PantinExport ThreeDSHeader
{
	kushort id; //chunk_id
	kulong size; //chunk_len
};


/*
 * Enumeration created regarding this webpage : http://www.the-labs.com/Blender/3DS-details.html
 *
 * The data types used by 3DS files :
 * - short is always a two byte integer.
 * - long is always a four byte integer.
 * - float is always a four byte IEEE floating point number.
 * - cstr is a zero byte terminated ASCII string without a length.
 * - char is a single byte integer.
 */
enum
{
	NULL_CHUNK = 0x0000,
	UNKNOWN_CHUNK = 0x0001,
	M3D_VERSION = 0x0002, //short version;
	M3D_KFVERSION = 0x0005,
	COLOR_F = 0x0010, //float red, grn, blu;
 	COLOR_24 = 0x0011, //char red, grn, blu;
 	LIN_COLOR_24 = 0x0012, //char red, grn, blu;
 	LIN_COLOR_F = 0x0013, //float red, grn, blu;
 	INT_PERCENTAGE = 0x0030, //short percentage;
 	FLOAT_PERCENTAGE = 0x0031, //float percentage;
 	MASTER_SCALE = 0x0100, //float scale;
 	CHUNK_TYPE = 0x0995,
 	CHUNK_UNIQUE = 0x0996,
 	NOT_CHUNK = 0x0997,
 	CONTAINER = 0x0998,
 	IS_CHUNK = 0x0999,
 	C_SXP_SELFI_MASKDATA = 0x0C3C,

 	BIT_MAP = 0x1100, //cstr filename;
 	USE_BIT_MAP = 0x1101,
 	SOLID_BGND = 0x1200, //followed by color_f
 	USE_SOLID_BGND = 0x1201, //float midpoint; followed by three color_f: start, mid, end
 	V_GRADIENT = 0x1300,
 	USE_V_GRADIENT = 0x1301,
 	LO_SHADOW_BIAS = 0x1400, //float bias;
 	HI_SHADOW_BIAS = 0x1410,
 	SHADOW_MAP_SIZE = 0x1420, //short size;
 	SHADOW_SAMPLES = 0x1430,
 	SHADOW_RANGE = 0x1440,
 	SHADOW_FILTER = 0x1450, //float filter;
 	RAY_BIAS = 0x1460, //float bias;
 	O_CONSTS = 0x1500, //float plane_x, plane_y, plane_z;

	AMBIENT_LIGHT = 0x2100,
	FOG = 0x2200, //float near_plane, near_density; float far_plane, far_density; followed by color_f, fog_bgnd
	USE_FOG = 0x2201,
	FOG_BGND = 0x2210,
	DISTANCE_CUE = 0x2300, //float near_plane, near_density; float far_plane, far_density; followed by dcue_bgnd
	USE_DISTANCE_CUE = 0x2301,
	LAYER_FOG = 0x2302, //float fog_z_from, fog_z_to; float fog_density; short fog_type;
	USE_LAYER_FOG = 0x2303,
	DCUE_BGND = 0x2310,
 	SMAGIC = 0x2D2D,
 	LMAGIC = 0x2D3D,

 	DEFAULT_VIEW = 0x3000,
 	VIEW_TOP = 0x3010, //float targe_x, target_y, target_z; float view_width;
 	VIEW_BOTTOM = 0x3020, //float targe_x, target_y, target_z; float view_width;
 	VIEW_LEFT = 0x3030, //float targe_x, target_y, target_z; float view_width;
 	VIEW_RIGHT = 0x3040, //float targe_x, target_y, target_z; float view_width;
 	VIEW_FRONT = 0x3050, //float targe_x, target_y, target_z; float view_width;
 	VIEW_BACK = 0x3060, //float targe_x, target_y, target_z; float view_width;
 	VIEW_USER = 0x3070, //float targe_x, target_y, target_z; float view_width;
 	VIEW_CAMERA = 0x3080, //cstr camera_name;
 	VIEW_WINDOW = 0x3090,
 	MDATA = 0x3D3D, //Mesh Data Magic Number (.3DS files sub of 4d4d)
 	MESH_VERSION = 0x3D3E,
 	MLIBMAGIC = 0x3DAA, //Material Library Magic Number (.MLI files)
 	PRJMAGIC = 0x3DC2, //3dS Project Magic Number (.PRJ files)
 	MATMAGIC = 0x3DFF, //Material File Magic Number (.MAT files)

 	NAMED_OBJECT = 0x4000, //cstr name;
 	OBJ_HIDDEN = 0x4010,
 	OBJ_VIS_LOFTER = 0x4011,
	OBJ_DOESNT_CAST = 0x4012,
	OBJ_MATTE = 0x4013,
 	OBJ_FAST = 0x4014,
 	OBJ_PROCEDURAL = 0x4015,
 	OBJ_FROZEN = 0x4016,
 	OBJ_DONT_RCVSHADOW = 0x4017,
 	N_TRI_OBJECT = 0x4100, //named triangle object followed by point_array, point_flag_array, mesh_matrix, face_array
 	POINT_ARRAY = 0x4110, //short npoints; struct { float x, y, z; } points[npoints];
	POINT_FLAG_ARRAY = 0x4111, //short nflags; short flags[nflags];
	FACE_ARRAY = 0x4120, //short nfaces; struct { short vertex1, vertex2, vertex3; short flags;	} facearray[nfaces]; may be followed by smooth_group
	MSH_MAT_GROUP = 0x4130, //mesh_material_group cstr material_name; short nfaces;	short facenum[nfaces];
	OLD_MAT_GROUP = 0x4131,
	TEX_VERTS = 0x4140, //short nverts;	struct { float x, y; } vertices[nverts];
	SMOOTH_GROUP = 0x4150, //short grouplist[n]; determined by length, seems to be 4 per face
	MESH_MATRIX = 0x4160, //float matrix[4][3];
	MESH_COLOR = 0x4165, //short color_index;
	MESH_TEXTURE_INFO = 0x4170, //short map_type; float x_tiling, y_tiling;	float icon_x, icon_y, icon_z; float matrix[4][3]; float scaling, plan_icon_w, plan_icon_h, cyl_icon_h;
	PROC_NAME = 0x4181,
	PROC_DATA = 0x4182,
	MSH_BOXMAP = 0x4190,
	N_D_L_OLD = 0x4400,
	N_CAM_OLD = 0x4500,
	N_DIRECT_LIGHT = 0x4600, //float x, y, z; followed by color_f
	DL_SPOTLIGHT = 0x4610, //float target_x, target_y, target_z; float hotspot_ang; float falloff_ang;
	DL_OFF = 0x4620,
	DL_ATTENUATE = 0x4625,
	DL_RAYSHAD = 0x4627,
	DL_SHADOWED = 0x4630,
	DL_LOCAL_SHADOW = 0x4640,
	DL_LOCAL_SHADOW2 = 0x4641,
	DL_SEE_CONE = 0x4650,
	DL_SPOT_RECTANGULAR = 0x4651,
	DL_SPOT_OVERSHOOT = 0x4652,
 	DL_SPOT_PROJECTOR = 0x4653,
 	DL_EXCLUDE = 0x4654,
	DL_RANGE = 0x4655,
	DL_SPOT_ROLL = 0x4656, //float roll_ang;
	DL_SPOT_ASPECT = 0x4657,
	DL_RAY_BIAS = 0x4658, //float bias;
	DL_INNER_RANGE = 0x4659, //float range;
	DL_OUTER_RANGE = 0x465A, //float range;
	DL_MULTIPLIER = 0x465B, //float multiple;
	N_AMBIENT_LIGHT = 0x4680,
	N_CAMERA = 0x4700, //float camera_x, camera_y, camera_z; float target_x, target_y, target_z; float bank_angle; float focus;
	CAM_SEE_CONE = 0x4710,
	CAM_RANGES = 0x4720, //float near_range, far_range;
	M3DMAGIC = 0x4D4D, //3DS Magic Number (.3DS file)
	HIERARCHY = 0x4F00,
	PARENT_OBJECT = 0x4F10,
	PIVOT_OBJECT = 0x4F20,
	PIVOT_LIMITS = 0x4F30,
	PIVOT_ORDER = 0x4F40,
	XLATE_RANGE = 0x4F50,

	POLY_2D = 0x5000,
 	SHAPE_OK = 0x5010,
 	SHAPE_NOT_OK = 0x5011,
 	SHAPE_HOOK = 0x5020,

 	PATH_3D = 0x6000,
 	PATH_MATRIX = 0x6005,
 	SHAPE_2D = 0x6010,
 	M_SCALE = 0x6020,
 	M_TWIST = 0x6030,
 	M_TEETER = 0x6040,
 	M_FIT = 0x6050,
 	M_BEVEL = 0x6060,
 	XZ_CURVE = 0x6070,
 	YZ_CURVE = 0x6080,
 	INTERPCT = 0x6090,
 	DEFORM_LIMIT = 0x60A0,
 	USE_CONTOUR = 0x6100,
 	USE_TWEEN = 0x6110,
 	USE_SCALE = 0x6020,
 	USE_TWIST = 0x6130,
 	USE_TEETER = 0x6140,
 	USE_FIT = 0x6150,
 	USE_BEVEL = 0x6160,

 	VIEWPORT_LAYOUT_OLD = 0x7000,
 	VIEWPORT_LAYOUT = 0x7001, //short form, top, ready, wstate, swapws, swapport, swapcur; followed by viewport_size, viewport_data
 	VIEWPORT_DATA_OLD = 0x7010,
 	VIEWPORT_DATA = 0x7011, //short flags, axis_lockout; short win_x, win_y, win_w, winh_, win_view; float zoom; float worldcenter_x, worldcenter_y, worldcenter_z; float horiz_ang, vert_ang; cstr camera_name;
 	VIEWPORT_DATA_3 = 0x7012, //short flags, axis_lockout; short win_x, win_y, win_w, winh_, win_view; float zoom; float worldcenter_x, worldcenter_y, worldcenter_z; float horiz_ang, vert_ang; cstr camera_name;
 	VIEWPORT_SIZE = 0x7020, //short x, y, w, h;
 	NETWORK_VIEW = 0x7030,

 	XDATA_SECTION = 0x8000,
 	XDATA_ENTRY = 0x8001,
 	XDATA_APPNAME = 0x8002,
 	XDATA_STRING = 0x8003,
 	XDATA_FLOAT = 0x8004,
 	XDATA_DOUBLE = 0x8005,
 	XDATA_SHORT = 0x8006,
 	XDATA_LONG = 0x8007,
 	XDATA_VOID = 0x8008,
 	XDATA_GROUP = 0x8009,
 	XDATA_RFU6 = 0x800A,
 	XDATA_RFU5 = 0x800B,
 	XDATA_RFU4 = 0x800C,
 	XDATA_RFU3 = 0x800D,
 	XDATA_RFU2 = 0x800E,
 	XDATA_RFU1 = 0x800F,
 	PARENT_NAME = 0x80F0,

	MAT_NAME = 0xA000, //cstr material_name;
 	MAT_AMBIENT = 0xA010, //followed by color chunk
 	MAT_DIFFUSE = 0xA020, //followed by color chunk
	MAT_SPECULAR = 0xA030, //followed by color chunk
	MAT_SHININESS = 0xA040, //followed by percentage chunk
	MAT_SHIN2PCT = 0xA041, //followed by percentage chunk
	MAT_TRANSPARENCY = 0xA050, //followed by percentage chunk
	MAT_XPFALL = 0xA052, //followed by percentage chunk
	MAT_REFBLUR = 0xA053, //followed by percentage chunk
	MAT_SELF_ILLUM = 0xA080,
	MAT_TWO_SIDE = 0xA081,
	MAT_DECAL = 0xA082,
	MAT_ADDITIVE = 0xA083,
	MAT_SELF_ILPCT = 0xA084, //followed by percentage chunk
	MAT_WIRE = 0xA085,
	MAT_SUPERSMP = 0xA086,
 	MAT_WIRESIZE = 0xA087, //float wire_size;
 	MAT_FACEMAP = 0xA088,
 	MAT_XPFALLIN = 0xA08A,
 	MAT_PHONGSOFT = 0xA08C,
 	MAT_WIREABS = 0xA08E,
	MAT_SHADING = 0xA100, //short shading_value;
	MAT_TEXMAP = 0xA200, //followed by percentage chunk, mat_mapname, mat_map_tiling, mat_map_texblur...
	MAT_SPECMAP = 0xA204, //followed by percentage_chunk, mat_mapname
	MAT_OPACMAP = 0xA210, //followed by percentage_chunk, mat_mapname
	MAT_REFLMAP = 0xA220, //followed by percentage_chunk, mat_mapname
	MAT_BUMPMAP = 0xA230, //followed by percentage_chunk, mat_mapname
	MAT_USE_XPFALL = 0xA240,
	MAT_USE_REFBLUR = 0xA250,
	MAT_BUMP_PERCENT = 0xA252,
	MAT_MAPNAME = 0xA300, //cstr filename;
	MAT_ACUBIC = 0xA310,
	MAT_SXP_TEXT_DATA = 0xA320,
	MAT_SXP_TEXT2_DATA = 0xA321,
	MAT_SXP_OPAC_DATA = 0xA322,
	MAT_SXP_BUMP_DATA = 0xA324,
	MAT_SXP_SPEC_DATA = 0xA325,
	MAT_SXP_SHIN_DATA = 0xA326,
	MAT_SXP_SELFI_DATA = 0xA328,
	MAT_SXP_TEXT_MASKDATA = 0xA32A,
	MAT_SXP_TEXT2_MASKDATA = 0xA32C,
	MAT_SXP_OPAC_MASKDATA = 0xA32E,
	MAT_SXP_BUMP_MASKDATA = 0xA330,
	MAT_SXP_SPEC_MASKDATA = 0xA332,
	MAT_SXP_SHIN_MASKDATA = 0xA334,
	MAT_SXP_SELFI_MASKDATA = 0xA336,
	MAT_SXP_REFL_MASKDATA = 0xA338,
	MAT_TEX2MAP = 0xA33A,
	MAT_SHINMAP = 0xA33C,
	MAT_SELFIMAP = 0xA33D,
	MAT_TEXMASK = 0xA33E,
	MAT_TEX2MASK = 0xA340,
	MAT_OPACMASK = 0xA342,
	MAT_BUMPMASK = 0xA344,
	MAT_SHINMASK = 0xA346,
	MAT_SPECMASK = 0xA348,
	MAT_SELFIMASK = 0xA34A,
	MAT_REFLMASK = 0xA34C,
	MAT_MAP_TILINGOLD = 0xA350,
	MAT_MAP_TILING = 0xA351, //short flags;
	MAT_MAP_TEXBLUR_OLD = 0xA352,
	MAT_MAP_TEXBLUR = 0xA353, //float blurring;
	MAT_MAP_USCALE = 0xA354,
	MAT_MAP_VSCALE = 0xA356,
	MAT_MAP_UOFFSET = 0xA358,
	MAT_MAP_VOFFSET = 0xA35A,
	MAT_MAP_ANG = 0xA35C,
	MAT_MAP_COL1 = 0xA360,
	MAT_MAP_COL2 = 0xA362,
	MAT_MAP_RCOL = 0xA364,
	MAT_MAP_GCOL = 0xA366,
	MAT_MAP_BCOL = 0xA368,
	MAT_ENTRY = 0xAFFF,

	KFDATA = 0xB000, //followed by kfhdr
	AMBIENT_NODE_TAG = 0xB001,
	OBJECT_NODE_TAG = 0xB002, //followed by node_hdr, pivot, pos_track_tag, rot_track_tag, scl_track_tag, morph_smooth...
	CAMERA_NODE_TAG = 0xB003, //followed by node_hdr, pos_track_tag, fov_track_tag, roll_track_tag...
	TARGET_NODE_TAG = 0xB004, //followed by node_hdr, pos_track_tag...
	LIGHT_NODE_TAG = 0xB005, //followed by node_hdr, pos_track_tag, col_track_tag...
	L_TARGET_NODE_TAG = 0xB006, //followed by node_id, node_hdr, pos_track_tag...
	SPOTLIGHT_NODE_TAG = 0xB007, //followed by node_id, node_hdr, pos_track_tag, hot_track_tag, fall_track_tag, roll_track_tag, col_track_tag...
	KFSEG = 0xB008, //short start, end;
	KFCURTIME = 0xB009, //short curframe;
	KFHDR = 0xB00A, //short revision; cstr filename; short animlen; followed by viewport_layout, kfseg, kfcurtime, object_node_tag, light_node_tag, target_node_tag, camera_node_tag, l_target_node_tag, spotlight_node_tag, ambient_node_tag...
	NODE_HDR = 0xB010, //cstr objname; short flags1; short flags2; short hierarchy;
	INSTANCE_NAME = 0xB011,
	PRESCALE = 0xB012,
	PIVOT = 0xB013, //float pivot_x, pivot_y, pivot_z;
	BOUNDBOX = 0xB014,
	MORPH_SMOOTH = 0xB015, //float morph_smoothing_angle_rad;
	POS_TRACK_TAG = 0xB020, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float pos_x, pos_y, pos_z; } pos[keys];
	ROT_TRACK_TAG = 0xB021, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float rotation_rad; float axis_x, axis_y, axis_z; } rot[keys];
	SCL_TRACK_TAG = 0xB022, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float scale_x, scale_y, scale_z; } scale[keys];
	FOV_TRACK_TAG = 0xB023, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float camera_field_of_view; } fov[keys];
	ROLL_TRACK_TAG = 0xB024, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float camera_roll; } roll[keys];
	COL_TRACK_TAG = 0xB025, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float red, rgn, blu; } color[keys];
 	MORPH_TRACK_TAG = 0xB026, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; cstr obj_name; } morph[keys];
 	HOT_TRACK_TAG = 0xB027, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float hotspot_ang; } hotspot[keys];
 	FALL_TRACK_TAG = 0xB028, //short flags; short unknown[4]; short keys; short unknown; struct { short framenum; long unknown; float falloff_ang; } falloff[keys];
 	HIDE_TRACK_TAG = 0xB029,
 	NODE_ID = 0xB030, //short id;

 	C_MDRAWER = 0xC010,
 	C_TDRAWER = 0xC020,
 	C_SHPDRAWER = 0xC030,
 	C_MODDRAWER = 0xC040,
 	C_RIPDRAWER = 0xC050,
 	C_TXDRAWER = 0xC060,
 	C_PDRAWER = 0xC062,
 	C_MTLDRAWER = 0xC064,
 	C_FLIDRAWER = 0xC066,
 	C_CUBDRAWER = 0xC067,
 	C_MFILE = 0xC070,
 	C_SHPFILE = 0xC080,
 	C_MODFILE = 0xC090,
 	C_RIPFILE = 0xC0A0,
 	C_TXFILE = 0xC0B0,
 	C_PFILE = 0xC0B2,
 	C_MTLFILE = 0xC0B4,
 	C_FLIFILE = 0xC0B6,
 	C_PALFILE = 0xC0B8,
 	C_TX_STRING = 0xC0C0,
 	C_CONSTS = 0xC0D0,
 	C_SNAPS = 0xC0E0,
 	C_GRIDS = 0xC0F0,
 	C_ASNAPS = 0xC100,
 	C_GRID_RANGE = 0xC110,
 	C_RENDTYPE = 0xC120,
 	C_PROGMODE = 0xC130,
 	C_PREVMODE = 0xC140,
 	C_MODWMODE = 0xC150,
 	C_MODMODEL = 0xC160,
 	C_ALL_LINES = 0xC170,
 	C_BACK_TYPE = 0xC180,
 	C_MD_CS = 0xC190,
 	C_MD_CE = 0xC1A0,
 	C_MD_SML = 0xC1B0,
 	C_MD_SMW = 0xC1C0,
 	C_LOFT_WITH_TEXTURE = 0xC1C3,
 	C_LOFT_L_REPEAT = 0xC1C4,
 	C_LOFT_W_REPEAT = 0xC1C5,
 	C_LOFT_UV_NORMALIZE = 0xC1C6,
 	C_WELD_LOFT = 0xC1C7,
 	C_MD_PDET = 0xC1D0,
 	C_MD_SDET = 0xC1E0,
 	C_RGB_RMODE = 0xC1F0,
 	C_RGB_HIDE = 0xC200,
 	C_RGB_MAPSW = 0xC202,
 	C_RGB_TWOSIDE = 0xC204,
 	C_RGB_SHADOW = 0xC208,
 	C_RGB_AA = 0xC210,
 	C_RGB_OVW = 0xC220,
 	C_RGB_OVH = 0xC230,
 	CMAGIC = 0xC23D,
 	C_RGB_PICTYPE = 0xC240,
 	C_RGB_OUTPUT = 0xC250,
 	C_RGB_TODISK = 0xC253,
 	C_RGB_COMPRESS = 0xC254,
 	C_JPEG_COMPRESSION = 0xC255,
 	C_RGB_DISPDEV = 0xC256,
 	C_RGB_HARDDEV = 0xC259,
 	C_RGB_PATH = 0xC25A,
 	C_BITMAP_DRAWER = 0xC25B,
 	C_RGB_FILE = 0xC260,
 	C_RGB_OVASPECT = 0xC270,
 	C_RGB_ANIMTYPE = 0xC271,
 	C_RENDER_ALL = 0xC272,
 	C_REND_FROM = 0xC273,
 	C_REND_TO = 0xC274,
 	C_REND_NTH = 0xC275,
 	C_PAL_TYPE = 0xC276,
 	C_RND_TURBO = 0xC277,
 	C_RND_MIP = 0xC278,
 	C_BGND_METHOD = 0xC279,
 	C_AUTO_REFLECT = 0xC27A,
 	C_VP_FROM = 0xC27B,
 	C_VP_TO = 0xC27C,
 	C_VP_NTH = 0xC27D,
 	C_REND_TSTEP = 0xC27E,
 	C_VP_TSTEP = 0xC27F,
 	C_SRDIAM = 0xC280,
 	C_SRDEG = 0xC290,
 	C_SRSEG = 0xC2A0,
 	C_SRDIR = 0xC2B0,
 	C_HETOP = 0xC2C0,
 	C_HEBOT = 0xC2D0,
 	C_HEHT = 0xC2E0,
 	C_HETURNS = 0xC2F0,
 	C_HEDEG = 0xC300,
 	C_HESEG = 0xC310,
 	C_HEDIR = 0xC320,
 	C_QUIKSTUFF = 0xC330,
 	C_SEE_LIGHTS = 0xC340,
 	C_SEE_CAMERAS = 0xC350,
 	C_SEE_3D = 0xC360,
 	C_MESHSEL = 0xC370,
 	C_MESHUNSEL = 0xC380,
 	C_POLYSEL = 0xC390,
 	C_POLYUNSEL = 0xC3A0,
 	C_SHPLOCAL = 0xC3A2,
 	C_MSHLOCAL = 0xC3A4,
 	C_NUM_FORMAT = 0xC3B0,
 	C_ARCH_DENOM = 0xC3C0,
 	C_IN_DEVICE = 0xC3D0,
 	C_MSCALE = 0xC3E0,
 	C_COMM_PORT = 0xC3F0,
 	C_TAB_BASES = 0xC400,
 	C_TAB_DIVS = 0xC410,
 	C_MASTER_SCALES = 0xC420,
 	C_SHOW_1STVERT = 0xC430,
 	C_SHAPER_OK = 0xC440,
 	C_LOFTER_OK = 0xC450,
 	C_EDITOR_OK = 0xC460,
 	C_KEYFRAMER_OK = 0xC470,
 	C_PICKSIZE = 0xC480,
 	C_MAPTYPE = 0xC490,
 	C_MAP_DISPLAY = 0xC4A0,
 	C_TILE_XY = 0xC4B0,
 	C_MAP_XYZ = 0xC4C0,
 	C_MAP_SCALE = 0xC4D0,
 	C_MAP_MATRIX_OLD = 0xC4E0,
 	C_MAP_MATRIX = 0xC4E1,
 	C_MAP_WID_HT = 0xC4F0,
 	C_OBNAME = 0xC500,
 	C_CAMNAME = 0xC510,
 	C_LTNAME = 0xC520,
 	C_CUR_MNAME = 0xC525,
 	C_CURMTL_FROM_MESH = 0xC526,
 	C_GET_SHAPE_MAKE_FACES = 0xC527,
 	C_DETAIL = 0xC530,
 	C_VERTMARK = 0xC540,
 	C_MSHAX = 0xC550,
 	C_MSHCP = 0xC560,
 	C_USERAX = 0xC570,
 	C_SHOOK = 0xC580,
 	C_RAX = 0xC590,
 	C_STAPE = 0xC5A0,
 	C_LTAPE = 0xC5B0,
 	C_ETAPE = 0xC5C0,
 	C_KTAPE = 0xC5C8,
 	C_SPHSEGS = 0xC5D0,
 	C_GEOSMOOTH = 0xC5E0,
 	C_HEMISEGS = 0xC5F0,
 	C_PRISMSEGS = 0xC600,
 	C_PRISMSIDES = 0xC610,
 	C_TUBESEGS = 0xC620,
 	C_TUBESIDES = 0xC630,
 	C_TORSEGS = 0xC640,
 	C_TORSIDES = 0xC650,
 	C_CONESIDES = 0xC660,
 	C_CONESEGS = 0xC661,
 	C_NGPARMS = 0xC670,
 	C_PTHLEVEL = 0xC680,
 	C_MSCSYM = 0xC690,
 	C_MFTSYM = 0xC6A0,
 	C_MTTSYM = 0xC6B0,
 	C_SMOOTHING = 0xC6C0,
 	C_MODICOUNT = 0xC6D0,
 	C_FONTSEL = 0xC6E0,
 	C_TESS_TYPE = 0xC6F0,
 	C_TESS_TENSION = 0xC6F1,
 	C_SEG_START = 0xC700,
 	C_SEG_END = 0xC705,
 	C_CURTIME = 0xC710,
 	C_ANIMLENGTH = 0xC715,
 	C_PV_FROM = 0xC720,
 	C_PV_TO = 0xC725,
 	C_PV_DOFNUM = 0xC730,
 	C_PV_RNG = 0xC735,
 	C_PV_NTH = 0xC740,
 	C_PV_TYPE = 0xC745,
 	C_PV_METHOD = 0xC750,
 	C_PV_FPS = 0xC755,
 	C_VTR_FRAMES = 0xC765,
 	C_VTR_HDTL = 0xC770,
 	C_VTR_HD = 0xC771,
 	C_VTR_TL = 0xC772,
 	C_VTR_IN = 0xC775,
 	C_VTR_PK = 0xC780,
 	C_VTR_SH = 0xC785,
 	C_WORK_MTLS = 0xC790,
 	C_WORK_MTLS_2 = 0xC792,
 	C_WORK_MTLS_3 = 0xC793,
 	C_WORK_MTLS_4 = 0xC794,
 	C_BGTYPE = 0xC7A1,
 	C_MEDTILE = 0xC7B0,
 	C_LO_CONTRAST = 0xC7D0,
 	C_HI_CONTRAST = 0xC7D1,
 	C_FROZ_DISPLAY = 0xC7E0,
 	C_BOOLWELD = 0xC7F0,
 	C_BOOLTYPE = 0xC7F1,
 	C_ANG_THRESH = 0xC900,
 	C_SS_THRESH = 0xC901,
 	C_TEXTURE_BLUR_DEFAULT = 0xC903,
 	C_MAPDRAWER = 0xCA00,
 	C_MAPDRAWER1 = 0xCA01,
 	C_MAPDRAWER2 = 0xCA02,
 	C_MAPDRAWER3 = 0xCA03,
 	C_MAPDRAWER4 = 0xCA04,
 	C_MAPDRAWER5 = 0xCA05,
 	C_MAPDRAWER6 = 0xCA06,
 	C_MAPDRAWER7 = 0xCA07,
 	C_MAPDRAWER8 = 0xCA08,
 	C_MAPDRAWER9 = 0xCA09,
 	C_MAPDRAWER_ENTRY = 0xCA10,
 	C_BACKUP_FILE = 0xCA20,
 	C_DITHER_256 = 0xCA21,
 	C_SAVE_LAST = 0xCA22,
 	C_USE_ALPHA = 0xCA23,
 	C_TGA_DEPTH = 0xCA24,
 	C_REND_FIELDS = 0xCA25,
 	C_REFLIP = 0xCA26,
 	C_SEL_ITEMTOG = 0xCA27,
 	C_SEL_RESET = 0xCA28,
 	C_STICKY_KEYINF = 0xCA29,
 	C_WELD_THRESHOLD = 0xCA2A,
 	C_ZCLIP_POINT = 0xCA2B,
 	C_ALPHA_SPLIT = 0xCA2C,
 	C_KF_SHOW_BACKFACE = 0xCA30,
 	C_OPTIMIZE_LOFT = 0xCA40,
 	C_TENS_DEFAULT = 0xCA42,
 	C_CONT_DEFAULT = 0xCA44,
 	C_BIAS_DEFAULT = 0xCA46,
 	C_DXFNAME_SRC = 0xCA50,
 	C_AUTO_WELD = 0xCA60,
 	C_AUTO_UNIFY = 0xCA70,
 	C_AUTO_SMOOTH = 0xCA80,
 	C_DXF_SMOOTH_ANG = 0xCA90,
 	C_SMOOTH_ANG = 0xCAA0,
 	C_WORK_MTLS_5 = 0xCB00,
 	C_WORK_MTLS_6 = 0xCB01,
 	C_WORK_MTLS_7 = 0xCB02,
 	C_WORK_MTLS_8 = 0xCB03,
 	C_WORKMTL = 0xCB04,
 	C_SXP_TEXT_DATA = 0xCB10,
 	C_SXP_OPAC_DATA = 0xCB11,
 	C_SXP_BUMP_DATA = 0xCB12,
 	C_SXP_SHIN_DATA = 0xCB13,
 	C_SXP_TEXT2_DATA = 0xCB20,
 	C_SXP_SPEC_DATA = 0xCB24,
 	C_SXP_SELFI_DATA = 0xCB28,
 	C_SXP_TEXT_MASKDATA = 0xCB30,
 	C_SXP_TEXT2_MASKDATA = 0xCB32,
 	C_SXP_OPAC_MASKDATA = 0xCB34,
 	C_SXP_BUMP_MASKDATA = 0xCB36,
 	C_SXP_SPEC_MASKDATA = 0xCB38,
 	C_SXP_SHIN_MASKDATA = 0xCB3A,
 	C_SXP_REFL_MASKDATA = 0xCB3E,
 	C_NET_USE_VPOST = 0xCC00,
 	C_NET_USE_GAMMA = 0xCC10,
 	C_NET_FIELD_ORDER = 0xCC20,
 	C_BLUR_FRAMES = 0xCD00,
 	C_BLUR_SAMPLES = 0xCD10,
 	C_BLUR_DUR = 0xCD20,
 	C_HOT_METHOD = 0xCD30,
 	C_HOT_CHECK = 0xCD40,
 	C_PIXEL_SIZE = 0xCD50,
 	C_DISP_GAMMA = 0xCD60,
 	C_FBUF_GAMMA = 0xCD70,
 	C_FILE_OUT_GAMMA = 0xCD80,
 	C_FILE_IN_GAMMA = 0xCD82,
 	C_GAMMA_CORRECT = 0xCD84,
 	C_APPLY_DISP_GAMMA = 0xCD90,
 	C_APPLY_FBUF_GAMMA = 0xCDA0,
 	C_APPLY_FILE_GAMMA = 0xCDB0,
 	C_FORCE_WIRE = 0xCDC0,
 	C_RAY_SHADOWS = 0xCDD0,
 	C_MASTER_AMBIENT = 0xCDE0,
 	C_SUPER_SAMPLE = 0xCDF0,
 	C_OBJECT_MBLUR = 0xCE00,
 	C_MBLUR_DITHER = 0xCE10,
 	C_DITHER_24 = 0xCE20,
 	C_SUPER_BLACK = 0xCE30,
 	C_SAFE_FRAME = 0xCE40,
 	C_VIEW_PRES_RATIO = 0xCE50,
 	C_BGND_PRES_RATIO = 0xCE60,
 	C_NTH_SERIAL_NUM = 0xCE70,

 	VPDATA = 0xD000,
 	P_QUEUE_ENTRY = 0xD100,
 	P_QUEUE_IMAGE = 0xD110,
 	P_QUEUE_USEIGAMMA = 0xD114,
 	P_QUEUE_PROC = 0xD120,
 	P_QUEUE_SOLID = 0xD130,
 	P_QUEUE_GRADIENT = 0xD140,
 	P_QUEUE_KF = 0xD150,
 	P_QUEUE_MOTBLUR = 0xD152,
 	P_QUEUE_MB_REPEAT = 0xD153,
 	P_QUEUE_NONE = 0xD160,
 	P_QUEUE_RESIZE = 0xD180,
 	P_QUEUE_OFFSET = 0xD185,
 	P_QUEUE_ALIGN = 0xD190,
 	P_CUSTOM_SIZE = 0xD1A0,
 	P_ALPH_NONE = 0xD210,
 	P_ALPH_PSEUDO = 0xD220,
 	P_ALPH_OP_PSEUDO = 0xD221,
 	P_ALPH_BLUR = 0xD222,
 	P_ALPH_PCOL = 0xD225,
 	P_ALPH_C0 = 0xD230,
 	P_ALPH_OP_KEY = 0xD231,
 	P_ALPH_KCOL = 0xD235,
 	P_ALPH_OP_NOCONV = 0xD238,
 	P_ALPH_IMAGE = 0xD240,
 	P_ALPH_ALPHA = 0xD250,
 	P_ALPH_QUES = 0xD260,
 	P_ALPH_QUEIMG = 0xD265,
 	P_ALPH_CUTOFF = 0xD270,
 	P_ALPHANEG = 0xD280,
 	P_TRAN_NONE = 0xD300,
 	P_TRAN_IMAGE = 0xD310,
 	P_TRAN_FRAMES = 0xD312,
 	P_TRAN_FADEIN = 0xD320,
 	P_TRAN_FADEOUT = 0xD330,
 	P_TRANNEG = 0xD340,
 	P_RANGES = 0xD400,
 	P_PROC_DATA = 0xD500,

 	POS_TRACK_TAG_KEY = 0xF020,
 	ROT_TRACK_TAG_KEY = 0xF021,
 	SCL_TRACK_TAG_KEY = 0xF022,
 	FOV_TRACK_TAG_KEY = 0xF023,
 	ROLL_TRACK_TAG_KEY = 0xF024,
 	COL_TRACK_TAG_KEY = 0xF025,
 	MORPH_TRACK_TAG_KEY = 0xF026,
 	HOT_TRACK_TAG_KEY = 0xF027,
 	FALL_TRACK_TAG_KEY = 0xF028,
 	POINT_ARRAY_ENTRY = 0xF110,
 	POINT_FLAG_ARRAY_ENTRY = 0xF111,
 	FACE_ARRAY_ENTRY = 0xF120,
 	MSH_MAT_GROUP_ENTRY = 0xF130,
 	TEX_VERTS_ENTRY = 0xF140,
 	SMOOTH_GROUP_ENTRY = 0xF150,
 	DUMMY = 0xFFFF,
};

}}}
